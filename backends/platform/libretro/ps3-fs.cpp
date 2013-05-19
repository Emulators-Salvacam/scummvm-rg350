/* ScummVM - Graphic Adventure Engine
*
* ScummVM is the legal property of its developers, whose names
* are too numerous to list here. Please refer to the COPYRIGHT
* file distributed with this source distribution.
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
*
*/

#include "backends/fs/abstract-fs.h"
#include "ps3-fs.h"
#include "ps3-filestream.h"
#include <cell/cell_fs.h>
#include <sys/paths.h>
#include "backends/fs/stdiostream.h"

#define FORBIDDEN_look_at_common_forbidden_h_for_more_info

Common::String _homedir_;

void setupHomeDir(const char *homedir)
{
	_homedir_=homedir;
}

const char *_lastPathComponent(const Common::String &str)
{
	if (str.empty())
		return "";

	const char *start = str.c_str();
	const char *cur = start + str.size() - 2;

	while (cur >= start && *cur != '/' && *cur != ':')
		--cur;

	cur++;

	return cur;
}

Ps3FilesystemNode::Ps3FilesystemNode()
{
	_isHere = true;
	_isDirectory = true;
	_verified = true;
	_path = _homedir_;
	_displayName = _lastPathComponent(_path);
}

Ps3FilesystemNode::Ps3FilesystemNode(const Common::String &path)
{
	if (path.empty() || path.equals("."))
	{
		_isHere = true;
		_isDirectory = true; /* root is always a dir */
		_displayName = Common::String("PlayStation 3");
		_verified = true;
		_path=_homedir_;
	}
	else
	{
		_path = path;
		_verified = false;
		doverify();
		if (!_isHere)
			return;

		_displayName = Common::String(_lastPathComponent(_path));

		if (_isDirectory && _path.lastChar() != '/')
			_path+= '/';
	}
}

Ps3FilesystemNode::Ps3FilesystemNode(const Common::String &path, bool verify)
{
	if (path.empty() || path.equals("."))
	{
		_isHere = true;
		_isDirectory = true; /* root is always a dir */
		_displayName = Common::String("PlayStation 3");
		_verified = true;
		_path=_homedir_;
	}
	else
	{
		_path = path;
		_verified = false;
		if (verify) {
			doverify();

			if (!_isHere)
				return;

		} else {
			_verified = false;
			_isDirectory = false;
			_isHere = false; // true
		}

		//printf("  --  _lastPathComponent");
		_displayName = Common::String(_lastPathComponent(_path));
		//printf(" %s\n",_displayName.c_str());

		if (_isDirectory && _path.lastChar() != '/')
		{
			//printf(" pathpadding...");
			_path+= '/';
			//printf(" done\n");
		}
		//printf("......\n");
	}
	//printf("::::::::\n");
}

Ps3FilesystemNode::Ps3FilesystemNode(const Ps3FilesystemNode *node)
{
	_displayName = node->_displayName;
	_isDirectory = node->_isDirectory;
	_path = node->_path;
	_isHere = node->_isHere;
	_verified = node->_verified;
}

void Ps3FilesystemNode::doverify(void)
{
	if (_verified)
		return;

	_verified = true;

	if (_path.empty() || _path[0] != '/')
		_path=_homedir_+_path;

	int fd=0;
	CellFsErrno err=0;
	err = cellFsOpen(_path.c_str(), CELL_FS_O_RDONLY, &fd, NULL, 0);

	if (err == CELL_FS_SUCCEEDED || err == CELL_FS_EISDIR)
	{
		cellFsClose(fd);
		_isHere = true;
		if (err == CELL_FS_EISDIR)
			_isDirectory = true;
		else
			_isDirectory = false;
		return;
	}
	if(fd>0)
		cellFsClose(fd);

	_isHere = false;
	_isDirectory = false;

	return;
}

AbstractFSNode *Ps3FilesystemNode::getChild(const Common::String &n) const
{
	if (!_isDirectory)
		return NULL;

	return new Ps3FilesystemNode(_path+n, 1);
}

bool Ps3FilesystemNode::getChildren(AbstractFSList &list, ListMode mode, bool hidden) const {
	if (!_isDirectory)
		return false;

	{
		CellFsErrno err;
		int fd;

		err=cellFsOpendir(_path.c_str(),&fd);

		if (err == CELL_FS_SUCCEEDED)
		{
			CellFsDirent dirent;
			uint64_t dreadRes;
			while (true)
			{
				err = cellFsReaddir(fd, &dirent,&dreadRes);
				if(dreadRes<1 || err!=CELL_FS_SUCCEEDED)
					break;

				if (dirent.d_name[0] == '.')
				{
					continue; // ignore '.' and '..'
				}

				if ( (mode == Common::FSNode::kListAll) ||

					((mode == Common::FSNode::kListDirectoriesOnly) &&
					(dirent.d_type & CELL_FS_TYPE_DIRECTORY)) ||

					((mode == Common::FSNode::kListFilesOnly) &&
					!(dirent.d_type & CELL_FS_TYPE_DIRECTORY)) )
				{

					Common::String newPath=_path+dirent.d_name;
					Ps3FilesystemNode *dirEntry=new Ps3FilesystemNode(newPath);
					list.push_back(dirEntry);
				}
				else
				{
//					printf("   ignoring: %s\n",dirent.d_name);
				}
			}
			cellFsClosedir(fd);
			return true;
		}
	}
	return false;
}

AbstractFSNode *Ps3FilesystemNode::getParent() const {
	const char *start = _path.c_str();
	const char *end = _lastPathComponent(_path);

	Common::String str(start, end - start);

	return new Ps3FilesystemNode(str, true);
}


Common::SeekableReadStream *Ps3FilesystemNode::createReadStream() {
	Common::SeekableReadStream *ss = StdioStream::makeFromPath(getPath().c_str(), false);
	return ss;
}

Common::WriteStream *Ps3FilesystemNode::createWriteStream() {
	return StdioStream::makeFromPath(getPath().c_str(), true);
}
