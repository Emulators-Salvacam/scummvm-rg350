#define FORBIDDEN_SYMBOL_ALLOW_ALL

#include "backends/modular-backend.h"
#include "base/main.h"
#include "common/scummsys.h"

#include "os.h"
#include "graphics.h"

#include "libco/libco.h"
#include "libretro.h"

#if 0
# define LOG(msg) fprintf(stderr, "%s\n", msg)
#else
# define LOG(msg)
#endif

//

static retro_video_refresh_t video_cb = NULL;
static retro_audio_sample_batch_t audio_batch_cb = NULL;
static retro_environment_t environ_cb = NULL;
static retro_input_poll_t poll_cb = NULL;
static retro_input_state_t input_cb = NULL;

void retro_set_video_refresh(retro_video_refresh_t cb) { video_cb = cb; }
void retro_set_audio_sample(retro_audio_sample_t cb) { }
void retro_set_audio_sample_batch(retro_audio_sample_batch_t cb) { audio_batch_cb = cb; }
void retro_set_input_poll(retro_input_poll_t cb) { poll_cb = cb; }
void retro_set_input_state(retro_input_state_t cb) { input_cb = cb; }
void retro_set_environment(retro_environment_t cb) { environ_cb = cb; }


//
static OSystem_Libretro* lrsystem;


bool FRONTENDwantsExit;
bool EMULATORwantsExit;

cothread_t mainThread;
cothread_t emuThread;

void retro_leave_thread()
{
    co_switch(mainThread);
}

static void retro_start_emulator()
{
    g_system = lrsystem = new OSystem_Libretro();

    static const char* argv[] = {"scummvm"};
    int res = scummvm_main(1, argv);
    delete lrsystem;
}

static void retro_wrap_emulator()
{
    retro_start_emulator();
            
    // Were done here
    co_switch(mainThread);
        
    // Dead emulator, but libco says not to return
    while(true)
    {
        LOG("Running a dead emulator.");
        co_switch(mainThread);
    }
}

//

unsigned retro_api_version(void)
{
   return RETRO_API_VERSION;
}

void retro_get_system_info(struct retro_system_info *info)
{
   info->library_name = "scummvm";
   info->library_version = "svn";
   info->valid_extensions = "exe|com|bat|conf|EXE|COM|BAT|CONF";
   info->need_fullpath = true;
   info->block_extract = false;
}

void retro_get_system_av_info(struct retro_system_av_info *info)
{
    // TODO
    info->geometry.base_width = 640;
    info->geometry.base_height = 400;
    info->geometry.max_width = 640;
    info->geometry.max_height = 480;
    info->geometry.aspect_ratio = 1.333333f;
    info->timing.fps = 60.0;
    info->timing.sample_rate = 44100.0;
}

void retro_init (void)
{
    // Get color mode: 32 first as VGA has 6 bits per pixel
/*    RDOSGFXcolorMode = RETRO_PIXEL_FORMAT_XRGB8888;
    if(!environ_cb(RETRO_ENVIRONMENT_SET_PIXEL_FORMAT, &RDOSGFXcolorMode))
    {
        RDOSGFXcolorMode = RETRO_PIXEL_FORMAT_RGB565;
        if(!environ_cb(RETRO_ENVIRONMENT_SET_PIXEL_FORMAT, &RDOSGFXcolorMode))
        {
            RDOSGFXcolorMode = RETRO_PIXEL_FORMAT_0RGB1555;
        }
    }*/


    if(!emuThread && !mainThread)
    {
        mainThread = co_active();
        emuThread = co_create(65536*sizeof(void*), retro_wrap_emulator);
    }
    else
    {
        LOG("retro_init called more than once.");
    }
}

void retro_deinit(void)
{
    FRONTENDwantsExit = !EMULATORwantsExit;
    
    if(emuThread)
    {
        // If the frontend says to exit we need to let the emulator run to finish its job.
        if(FRONTENDwantsExit)
        {
            co_switch(emuThread);
        }
        
        co_delete(emuThread);
        emuThread = 0;
    }
    else
    {
        LOG("retro_deinit called when there is no emulator thread.");
    }
}

bool retro_load_game(const struct retro_game_info *game)
{
    if(emuThread)
    {
        return true;
    }
    else
    {
        LOG("retro_load_game called when there is no emulator thread.");
        return false;
    }
}

bool retro_load_game_special(unsigned game_type, const struct retro_game_info *info, size_t num_info)
{
    return false;
}

void retro_run (void)
{
    if(emuThread)
    {
        poll_cb();
    
        // Mouse
        if(lrsystem)
        {
            const int16_t mouseX = input_cb(1, RETRO_DEVICE_MOUSE, 0, RETRO_DEVICE_ID_MOUSE_X);
            const int16_t mouseY = input_cb(1, RETRO_DEVICE_MOUSE, 0, RETRO_DEVICE_ID_MOUSE_Y);
            const bool mouseLeft = input_cb(1, RETRO_DEVICE_MOUSE, 0, RETRO_DEVICE_ID_MOUSE_LEFT) ? true : false;
            lrsystem->sendMouseData(mouseX, mouseY, mouseLeft);
        }
    
        // Run emu
        co_switch(emuThread);
    
        // Upload video: TODO: Check the CANDUPE env value
        video_cb(lrsystem->getRetroGraphics()->getScreen(), 640, 480, 640 * 2);
    
        // Upload audio: TODO: Support sample rate control
        static uint32 buf[48000];
        ((Audio::MixerImpl*)lrsystem->getMixer())->mixCallback((byte*)buf, 24000);
//        audio_batch_cb((int16_t*)audioData, samplesPerFrame);
    }
    else
    {
        LOG("retro_run called when there is no emulator thread.");
    }
}

// Stubs
void retro_set_controller_port_device(unsigned in_port, unsigned device) { }
void *retro_get_memory_data(unsigned type) { return 0; }
size_t retro_get_memory_size(unsigned type) { return 0; }
void retro_reset (void) { }
size_t retro_serialize_size (void) { return 0; }
bool retro_serialize(void *data, size_t size) { return false; }
bool retro_unserialize(const void * data, size_t size) { return false; }
void retro_cheat_reset(void) { }
void retro_cheat_set(unsigned unused, bool unused1, const char* unused2) { }
void retro_unload_game (void) { }
unsigned retro_get_region (void) { return RETRO_REGION_NTSC; }
