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

// Heavily inspired by hoc
// Copyright (C) AT&T 1995
// All Rights Reserved
//
// Permission to use, copy, modify, and distribute this software and
// its documentation for any purpose and without fee is hereby
// granted, provided that the above copyright notice appear in all
// copies and that both that the copyright notice and this
// permission notice and warranty disclaimer appear in supporting
// documentation, and that the name of AT&T or any of its entities
// not be used in advertising or publicity pertaining to
// distribution of the software without specific, written prior
// permission.
//
// AT&T DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
// INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS.
// IN NO EVENT SHALL AT&T OR ANY OF ITS ENTITIES BE LIABLE FOR ANY
// SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
// WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER
// IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
// ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF
// THIS SOFTWARE.

#include "director/director.h"
#include "director/cast.h"
#include "director/score.h"
#include "director/util.h"
#include "director/lingo/lingo.h"
#include "director/lingo/lingo-builtins.h"
#include "director/lingo/lingo-code.h"

namespace Director {

static struct FuncDescr {
	const inst func;
	const char *name;
	const char *args;
} funcDescr[] = {
	{ 0,					"STOP",				""  },
	{ LC::c_xpop,			"c_xpop",			""  },
	{ LC::c_argcpush,		"c_argcpush",		"i" },
	{ LC::c_argcnoretpush,	"c_argcnoretpush",	"i" },
	{ LC::c_arraypush,		"c_arraypush",		"i" },
	{ LC::c_printtop,		"c_printtop",		""  },
	{ LC::c_intpush,		"c_intpush",		"i" },
	{ LC::c_voidpush,		"c_voidpush",		""  },
	{ LC::c_floatpush,		"c_floatpush",		"f" },
	{ LC::c_stringpush,		"c_stringpush",		"s" },
	{ LC::c_symbolpush,		"c_symbolpush",		"s" },	// D3
	{ LC::c_namepush,		"c_namepush",		"i" },
	{ LC::c_varpush,		"c_varpush",		"s" },
	{ LC::c_setImmediate,	"c_setImmediate",	"i" },
	{ LC::c_assign,			"c_assign",			""  },
	{ LC::c_eval,			"c_eval",			"s" },
	{ LC::c_theentitypush,	"c_theentitypush",	"ii" }, // entity, field
	{ LC::c_theentityassign,"c_theentityassign","ii" },
	{ LC::c_objectfieldpush,"c_objectfieldpush","si" }, // object, field
	{ LC::c_objectfieldassign,"c_objectfieldassign","si" },
	{ LC::c_swap,			"c_swap",			"" },
	{ LC::c_add,			"c_add",			"" },
	{ LC::c_sub,			"c_sub",			"" },
	{ LC::c_mul,			"c_mul",			"" },
	{ LC::c_div,			"c_div",			"" },
	{ LC::c_mod,			"c_mod",			"" },
	{ LC::c_negate,			"c_negate",			"" },
	{ LC::c_ampersand,		"c_ampersand",		"" },
	{ LC::c_after,			"c_after",			"" },	// D3
	{ LC::c_before,			"c_before",			"" },	// D3
	{ LC::c_concat,			"c_concat",			"" },
	{ LC::c_contains,		"c_contains",		"" },
	{ LC::c_starts,			"c_starts",			"" },
	{ LC::c_intersects,		"c_intersects",		"" },
	{ LC::c_within,			"c_within",			"" },
	{ LC::c_of,				"c_of",				"" },
	{ LC::c_charOf,			"c_charOf",			"" },	// D3
	{ LC::c_charToOf,		"c_charToOf",		"" },	// D3
	{ LC::c_itemOf,			"c_itemOf",			"" },	// D3
	{ LC::c_itemToOf,		"c_itemToOf",		"" },	// D3
	{ LC::c_lineOf,			"c_lineOf",			"" },	// D3
	{ LC::c_lineToOf,		"c_lineToOf",		"" },	// D3
	{ LC::c_wordOf,			"c_wordOf",			"" },	// D3
	{ LC::c_wordToOf,		"c_wordToOf",		"" },	// D3
	{ LC::c_and,			"c_and",			"" },
	{ LC::c_or,				"c_or",				"" },
	{ LC::c_not,			"c_not",			"" },
	{ LC::c_eq,				"c_eq",				"" },
	{ LC::c_neq,			"c_neq",			"" },
	{ LC::c_gt,				"c_gt",				"" },
	{ LC::c_lt,				"c_lt",				"" },
	{ LC::c_ge,				"c_ge",				"" },
	{ LC::c_le,				"c_le",				"" },
	{ LC::c_jump,			"c_jump",			"i" },
	{ LC::c_jumpifz,		"c_jumpifz",		"i" },
	{ LC::c_repeatwhilecode,"c_repeatwhilecode","oo" },
	{ LC::c_repeatwithcode,	"c_repeatwithcode",	"ooooos" },
	{ LC::c_exitRepeat,		"c_exitRepeat",		"" },
	{ LC::c_ifcode,			"c_ifcode",			"oooi" },
	{ LC::c_tellcode,		"c_tellcode",		"o" },
	{ LC::c_tell,			"c_tell",			"" },
	{ LC::c_telldone,		"c_telldone",		"" },
	{ LC::c_whencode,		"c_whencode",		"os" },
	{ LC::c_goto,			"c_goto",			"" },
	{ LC::c_gotoloop,		"c_gotoloop",		"" },
	{ LC::c_gotonext,		"c_gotonext",		"" },
	{ LC::c_gotoprevious,	"c_gotoprevious",	"" },
	{ LC::c_play,			"c_play",			"" },
	{ LC::c_playdone,		"c_playdone",		"" },
	{ LC::c_call,			"c_call",			"si" },
	{ LC::c_procret,		"c_procret",		"" },
	{ LC::c_global,			"c_global",			"s" },
	{ LC::c_property,		"c_property",		"s" },
	{ LC::c_instance,		"c_instance",		"s" },
	{ LC::c_open,			"c_open",			"" },
	{ LC::c_hilite,			"c_hilite",			"" },
	{ LC::c_unk,			"c_unk",			"i" },
	{ LC::c_unk1,			"c_unk1",			"ii" },
	{ LC::c_unk2,			"c_unk2",			"iii" },
	{ LC::cb_call,			"cb_call",			"i" },
	{ LC::cb_field,			"cb_field",			"" },
	{ LC::cb_localcall,		"cb_localcall",		"i" },
	{ LC::cb_v4putvalue,		"cb_putvalue",		"i" },
	{ LC::cb_v4theentitypush,"c_v4theentitypush","i" },
	{ LC::cb_v4theentitynamepush,"c_v4theentitynamepush","i" },
	{ LC::cb_v4theentityassign,"c_v4theentityassign","i" },
	{ 0, 0, 0 }
};

void Lingo::initFuncs() {
	Symbol sym;
	for (FuncDescr *fnc = funcDescr; fnc->name; fnc++) {
		sym.u.func = fnc->func;
		_functions[(void *)sym.u.s] = new FuncDesc(fnc->name, fnc->args);
	}
}

void Lingo::push(Datum d) {
	_stack.push_back(d);
}

void Lingo::pushVoid() {
	Datum d;
	d.u.s = NULL;
	d.type = VOID;
	push(d);
}

Datum Lingo::pop(void) {
	assert (_stack.size() != 0);

	Datum ret = _stack.back();
	_stack.pop_back();

	return ret;
}

void LC::c_xpop() {
	g_lingo->pop();
}

void LC::c_printtop(void) {
	Datum d = g_lingo->pop();

	switch (d.type) {
	case VOID:
		warning("Void, came from %s", d.u.s ? d.u.s->c_str() : "<>");
		break;
	case INT:
		warning("%d", d.u.i);
		break;
	case FLOAT:
		warning(g_lingo->_floatPrecisionFormat.c_str(), d.u.f);
		break;
	case VAR:
		if (!d.u.sym) {
			warning("Inconsistent stack: var, val: %d", d.u.i);
		} else {
			if (!d.u.sym->name.empty())
				warning("var: %s", d.u.sym->name.c_str());
			else
				warning("Nameless var. val: %d", d.u.sym->u.i);
		}
		break;
	case STRING:
		warning("%s", d.u.s->c_str());
		break;
	case POINT:
		warning("point(%d, %d)", (int)((*d.u.farr)[0]), (int)((*d.u.farr)[1]));
		break;
	case SYMBOL:
		warning("%s", d.type2str(true));
		break;
	case OBJECT:
		warning("#%s", d.u.s->c_str());
		break;
	default:
		warning("--unknown--");
	}
}

void LC::c_intpush() {
	Datum d;
	d.u.i = g_lingo->readInt();
	d.type = INT;
	g_lingo->push(d);
}

void LC::c_voidpush() {
	Datum d;
	d.u.s = NULL;
	d.type = VOID;
	g_lingo->push(d);
}

void LC::c_floatpush() {
	Datum d;
	d.u.f = g_lingo->readFloat();
	d.type = FLOAT;
	g_lingo->push(d);
}

void LC::c_stringpush() {
	char *s = g_lingo->readString();

	g_lingo->push(Datum(new Common::String(s)));
}

void LC::c_symbolpush() {
	char *s = g_lingo->readString();

	warning("STUB: c_symbolpush()");

	// TODO: FIXME: Must push symbol instead of string
	g_lingo->push(Datum(new Common::String(s)));
}

void LC::c_namepush() {
	Datum d;
	int i = g_lingo->readInt();
	g_lingo->push(Datum(new Common::String(g_lingo->_namelist[i])));
}

void LC::c_argcpush() {
	Datum d;
	int argsSize = g_lingo->readInt();

	d.u.i = argsSize;
	d.type = ARGC;
	g_lingo->push(d);
}

void LC::c_argcnoretpush() {
	Datum d;
	int argsSize = g_lingo->readInt();

	d.u.i = argsSize;
	d.type = ARGCNORET;
	g_lingo->push(d);
}

void LC::c_arraypush() {
	Datum d;
	int arraySize = g_lingo->readInt();

	warning("STUB: c_arraypush()");

	for (int i = 0; i < arraySize; i++)
		g_lingo->pop();

	d.u.i = arraySize;
	d.type = INT;
	g_lingo->push(d);
}

void LC::c_varpush() {
	Common::String name(g_lingo->readString());
	Datum d;

	// In immediate mode we will push variables as strings
	// This is used for playAccel
	if (g_lingo->_immediateMode) {
		g_lingo->push(Datum(new Common::String(name)));

		return;
	}

	if (g_lingo->getHandler(name) != NULL) {
		d.type = HANDLER;
		d.u.s = new Common::String(name);
		g_lingo->push(d);
		return;
	}

	d.u.sym = g_lingo->lookupVar(name.c_str());
	if (d.u.sym->type == CASTREF) {
		d.type = INT;
		int val = d.u.sym->u.i;

		delete d.u.sym;

		d.u.i = val;
	} else {
		d.type = VAR;
	}

	g_lingo->push(d);
}

void LC::c_setImmediate() {
	g_lingo->_immediateMode = g_lingo->readInt();
}

void LC::c_assign() {
	Datum d1, d2;
	d1 = g_lingo->pop();
	d2 = g_lingo->pop();

	if (d1.type != VAR && d1.type != REFERENCE) {
		warning("assignment to non-variable");
		return;
	}

	if (d1.type == REFERENCE) {
		Score *score = g_director->getCurrentScore();
		if (!score->_loadedCast->contains(d1.u.i)) {
			if (!score->_loadedCast->contains(d1.u.i - score->_castIDoffset)) {
				warning("c_assign: Unknown REFERENCE %d", d1.u.i);
				g_lingo->pushVoid();
				return;
			} else {
				d1.u.i -= score->_castIDoffset;
			}
		}

		d2.toString();

		((TextCast *)score->_loadedCast->getVal(d1.u.i))->setText(d2.u.s->c_str());

		return;
	}

	if (d1.u.sym->type != INT && d1.u.sym->type != VOID &&
			d1.u.sym->type != FLOAT && d1.u.sym->type != STRING) {
		warning("assignment to non-variable '%s'", d1.u.sym->name.c_str());
		return;
	}

	if ((d1.u.sym->type == STRING || d1.u.sym->type == VOID) && d1.u.sym->u.s) // Free memory if needed
		delete d1.u.sym->u.s;

	if (d1.u.sym->type == POINT || d1.u.sym->type == RECT || d1.u.sym->type == ARRAY)
		delete d1.u.sym->u.farr;

	if (d2.type == INT) {
		d1.u.sym->u.i = d2.u.i;
	} else if (d2.type == FLOAT) {
		d1.u.sym->u.f = d2.u.f;
	} else if (d2.type == STRING) {
		d1.u.sym->u.s = new Common::String(*d2.u.s);
		delete d2.u.s;
	} else if (d2.type == POINT) {
		d1.u.sym->u.farr = new FloatArray(*d2.u.farr);
		delete d2.u.farr;
	} else if (d2.type == SYMBOL) {
		d1.u.sym->u.i = d2.u.i;
	} else if (d2.type == OBJECT) {
		d1.u.sym->u.s = d2.u.s;
	} else {
		warning("c_assign: unhandled type: %s", d2.type2str());
		d1.u.sym->u.s = d2.u.s;
	}

	d1.u.sym->type = d2.type;
}

bool LC::verify(Symbol *s) {
	if (s->type != INT && s->type != VOID && s->type != FLOAT && s->type != STRING && s->type != POINT && s->type != SYMBOL) {
		warning("attempt to evaluate non-variable '%s'", s->name.c_str());

		return false;
	}

	if (s->type == VOID)
		warning("Variable used before assigning a value '%s'", s->name.c_str());

	return true;
}

void LC::c_eval() {
	LC::c_varpush();

	Datum d;
	d = g_lingo->pop();

	if (d.type == HANDLER) {
		LC::call(*d.u.s, 0);
		delete d.u.s;
		return;
	}

	if (d.type != VAR) { // It could be cast ref
		g_lingo->push(d);
		return;
	}

	if (!LC::verify(d.u.sym))
		return;

	d.type = d.u.sym->type;

	if (d.u.sym->type == INT)
		d.u.i = d.u.sym->u.i;
	else if (d.u.sym->type == FLOAT)
		d.u.f = d.u.sym->u.f;
	else if (d.u.sym->type == STRING)
		d.u.s = new Common::String(*d.u.sym->u.s);
	else if (d.u.sym->type == POINT)
		d.u.farr = d.u.sym->u.farr;
	else if (d.u.sym->type == SYMBOL)
		d.u.i = d.u.sym->u.i;
	else if (d.u.sym->type == VOID)
		d.u.s = new Common::String(d.u.sym->name);
	else
		warning("c_eval: unhandled type: %s", d.type2str());

	g_lingo->push(d);
}

void LC::c_theentitypush() {
	Datum id = g_lingo->pop();

	int entity = g_lingo->readInt();
	int field  = g_lingo->readInt();

	Datum d = g_lingo->getTheEntity(entity, id, field);
	g_lingo->push(d);
}

void LC::c_theentityassign() {
	Datum id = g_lingo->pop();

	int entity = g_lingo->readInt();
	int field  = g_lingo->readInt();

	Datum d = g_lingo->pop();
	g_lingo->setTheEntity(entity, id, field, d);
}

void LC::c_objectfieldpush() {
	Common::String object(g_lingo->readString());
	int field  = g_lingo->readInt();

	Datum d = g_lingo->getObjectField(object, field);
	g_lingo->push(d);
}

void LC::c_objectfieldassign() {
	Common::String object(g_lingo->readString());
	int field  = g_lingo->readInt();

	Datum d = g_lingo->pop();

	g_lingo->setObjectField(object, field, d);
}

void LC::c_swap() {
	Datum d2 = g_lingo->pop();
	Datum d1 = g_lingo->pop();
	g_lingo->push(d2);
	g_lingo->push(d1);
}

void LC::c_add() {
	Datum d2 = g_lingo->pop();
	Datum d1 = g_lingo->pop();

	if (g_lingo->alignTypes(d1, d2) == FLOAT) {
		d1.u.f += d2.u.f;
	} else {
		d1.u.i += d2.u.i;
	}
	g_lingo->push(d1);
}

void LC::c_sub() {
	Datum d2 = g_lingo->pop();
	Datum d1 = g_lingo->pop();

	if (g_lingo->alignTypes(d1, d2) == FLOAT) {
		d1.u.f -= d2.u.f;
	} else {
		d1.u.i -= d2.u.i;
	}
	g_lingo->push(d1);
}

void LC::c_mul() {
	Datum d2 = g_lingo->pop();
	Datum d1 = g_lingo->pop();

	if (g_lingo->alignTypes(d1, d2) == FLOAT) {
		d1.u.f *= d2.u.f;
	} else {
		d1.u.i *= d2.u.i;
	}
	g_lingo->push(d1);
}

void LC::c_div() {
	Datum d2 = g_lingo->pop();

	if ((d2.type == INT && d2.u.i == 0) ||
			(d2.type == FLOAT && d2.u.f == 0.0))
		error("division by zero");

	Datum d1 = g_lingo->pop();

	if (g_lingo->alignTypes(d1, d2) == FLOAT) {
		d1.u.f /= d2.u.f;
	} else {
		d1.u.i /= d2.u.i;
	}
	g_lingo->push(d1);
}

void LC::c_mod() {
	Datum d2 = g_lingo->pop();
	d2.toInt();

	if (d2.u.i == 0)
		error("division by zero");

	Datum d1 = g_lingo->pop();
	d1.toInt();

	d1.u.i %= d2.u.i;

	g_lingo->push(d1);
}

void LC::c_negate() {
	Datum d = g_lingo->pop();

	if (d.type == INT)
		d.u.i = -d.u.i;
	else if (d.type == FLOAT)
		d.u.f = -d.u.f;

	g_lingo->push(d);
}

void LC::c_ampersand() {
	Datum d2 = g_lingo->pop();
	Datum d1 = g_lingo->pop();

	d1.toString();
	d2.toString();

	*d1.u.s += *d2.u.s;

	delete d2.u.s;

	g_lingo->push(d1);
}

void LC::c_after() {
	Datum d2 = g_lingo->pop();
	Datum d1 = g_lingo->pop();

	d1.toString();
	d2.toString();

	warning("STUB: c_after");

	delete d2.u.s;

	g_lingo->push(d1);
}

void LC::c_before() {
	Datum d2 = g_lingo->pop();
	Datum d1 = g_lingo->pop();

	d1.toString();
	d2.toString();

	warning("STUB: c_before");

	delete d2.u.s;

	g_lingo->push(d1);
}

void LC::c_concat() {
	Datum d2 = g_lingo->pop();
	Datum d1 = g_lingo->pop();

	d1.toString();
	d2.toString();

	*d1.u.s += " ";
	*d1.u.s += *d2.u.s;

	delete d2.u.s;

	g_lingo->push(d1);
}

void LC::c_contains() {
	Datum d2 = g_lingo->pop();
	Datum d1 = g_lingo->pop();

	d1.toString();
	d2.toString();

	Common::String *s1 = toLowercaseMac(d1.u.s);
	Common::String *s2 = toLowercaseMac(d2.u.s);

	int res = s1->contains(*s2) ? 1 : 0;

	delete d1.u.s;
	delete d2.u.s;
	delete s1;
	delete s2;

	d1.type = INT;
	d1.u.i = res;

	g_lingo->push(d1);
}

void LC::c_starts() {
	Datum d2 = g_lingo->pop();
	Datum d1 = g_lingo->pop();

	d1.toString();
	d2.toString();

	Common::String *s1 = toLowercaseMac(d1.u.s);
	Common::String *s2 = toLowercaseMac(d2.u.s);

	int res = s1->hasPrefix(*s2) ? 1 : 0;

	delete d1.u.s;
	delete d2.u.s;
	delete s1;
	delete s2;

	d1.type = INT;
	d1.u.i = res;

	g_lingo->push(d1);
}

void LC::c_intersects() {
	Datum d2 = g_lingo->pop();
	Datum d1 = g_lingo->pop();

	warning("STUB: c_intersects: %d %d", d1.u.i, d2.u.i);

	g_lingo->push(d1);
}

void LC::c_within() {
	Datum d2 = g_lingo->pop();
	Datum d1 = g_lingo->pop();

	warning("STUB: c_within: %d %d", d1.u.i, d2.u.i);

	g_lingo->push(d1);
}

void LC::c_of() {
	Datum first_char = g_lingo->pop();
	Datum last_char = g_lingo->pop();
	Datum first_word = g_lingo->pop();
	Datum last_word = g_lingo->pop();
	Datum first_item = g_lingo->pop();
	Datum last_item = g_lingo->pop();
	Datum first_line = g_lingo->pop();
	Datum last_line = g_lingo->pop();
	Datum target = g_lingo->pop();

	warning("STUB: c_of: %d %d %d %d %d %d %d %d %s",
		first_char.u.i, last_char.u.i, first_word.u.i, last_word.u.i,
		first_item.u.i, last_item.u.i, first_line.u.i, last_line.u.i,
		target.u.s->c_str());

	g_lingo->push(target);

}

void LC::c_charOf() {
	Datum d2 = g_lingo->pop();
	Datum d1 = g_lingo->pop();

	warning("STUB: c_charOf: %d %d", d1.u.i, d2.u.i);

	g_lingo->push(d1);
}

void LC::c_charToOf() {
	Datum d3 = g_lingo->pop();
	Datum d2 = g_lingo->pop();
	Datum d1 = g_lingo->pop();

	warning("STUB: c_charToOf: %d %d %d", d1.u.i, d2.u.i, d3.u.i);

	g_lingo->push(d1);
}

void LC::c_itemOf() {
	Datum d2 = g_lingo->pop();
	Datum d1 = g_lingo->pop();

	warning("STUB: c_itemOf: %d %d", d1.u.i, d2.u.i);

	g_lingo->push(d1);
}

void LC::c_itemToOf() {
	Datum d3 = g_lingo->pop();
	Datum d2 = g_lingo->pop();
	Datum d1 = g_lingo->pop();

	warning("STUB: c_itemToOf: %d %d %d", d1.u.i, d2.u.i, d3.u.i);

	g_lingo->push(d1);
}

void LC::c_lineOf() {
	Datum d2 = g_lingo->pop();
	Datum d1 = g_lingo->pop();

	warning("STUB: c_lineOf: %d %d", d1.u.i, d2.u.i);

	g_lingo->push(d1);
}

void LC::c_lineToOf() {
	Datum d3 = g_lingo->pop();
	Datum d2 = g_lingo->pop();
	Datum d1 = g_lingo->pop();

	warning("STUB: c_lineToOf: %d %d %d", d1.u.i, d2.u.i, d3.u.i);

	g_lingo->push(d1);
}

void LC::c_wordOf() {
	Datum d2 = g_lingo->pop();
	Datum d1 = g_lingo->pop();

	warning("STUB: c_wordOf: %d %d", d1.u.i, d2.u.i);

	g_lingo->push(d1);
}

void LC::c_wordToOf() {
	Datum d3 = g_lingo->pop();
	Datum d2 = g_lingo->pop();
	Datum d1 = g_lingo->pop();

	warning("STUB: c_wordToOf: %d %d %d", d1.u.i, d2.u.i, d3.u.i);

	g_lingo->push(d1);
}

void LC::c_and() {
	Datum d2 = g_lingo->pop();
	Datum d1 = g_lingo->pop();

	d1.toInt();
	d2.toInt();

	d1.u.i = (d1.u.i && d2.u.i) ? 1 : 0;

	g_lingo->push(d1);
}

void LC::c_or() {
	Datum d2 = g_lingo->pop();
	Datum d1 = g_lingo->pop();

	d1.toInt();
	d2.toInt();

	d1.u.i = (d1.u.i || d2.u.i) ? 1 : 0;

	g_lingo->push(d1);
}

void LC::c_not() {
	Datum d = g_lingo->pop();

	d.toInt();

	d.u.i = ~d.u.i ? 1 : 0;

	g_lingo->push(d);
}

void LC::c_eq() {
	Datum d2 = g_lingo->pop();
	Datum d1 = g_lingo->pop();

	if (d1.type == STRING && d2.type == STRING) {
		d1.u.i = (d1.u.s->equalsIgnoreCase(*d2.u.s)) ? 1 : 0;
		d1.type = INT;
	} else if (g_lingo->alignTypes(d1, d2) == FLOAT) {
		d1.u.i = (d1.u.f == d2.u.f) ? 1 : 0;
		d1.type = INT;
	} else {
		d1.u.i = (d1.u.i == d2.u.i) ? 1 : 0;
	}
	g_lingo->push(d1);
}

void LC::c_neq() {
	Datum d2 = g_lingo->pop();
	Datum d1 = g_lingo->pop();

	if (d1.type == STRING && d2.type == STRING) {
		d1.u.i = !(d1.u.s->equalsIgnoreCase(*d2.u.s)) ? 1 : 0;
		d1.type = INT;
	} else if (g_lingo->alignTypes(d1, d2) == FLOAT) {
		d1.u.i = (d1.u.f != d2.u.f) ? 1 : 0;
		d1.type = INT;
	} else {
		d1.u.i = (d1.u.i != d2.u.i) ? 1 : 0;
	}
	g_lingo->push(d1);
}

void LC::c_gt() {
	Datum d2 = g_lingo->pop();
	Datum d1 = g_lingo->pop();

	if (g_lingo->alignTypes(d1, d2) == FLOAT) {
		d1.u.i = (d1.u.f > d2.u.f) ? 1 : 0;
		d1.type = INT;
	} else {
		d1.u.i = (d1.u.i > d2.u.i) ? 1 : 0;
	}
	g_lingo->push(d1);
}

void LC::c_lt() {
	Datum d2 = g_lingo->pop();
	Datum d1 = g_lingo->pop();

	if (g_lingo->alignTypes(d1, d2) == FLOAT) {
		d1.u.i = (d1.u.f < d2.u.f) ? 1 : 0;
		d1.type = INT;
	} else {
		d1.u.i = (d1.u.i < d2.u.i) ? 1 : 0;
	}
	g_lingo->push(d1);
}

void LC::c_ge() {
	Datum d2 = g_lingo->pop();
	Datum d1 = g_lingo->pop();

	if (g_lingo->alignTypes(d1, d2) == FLOAT) {
		d1.u.i = (d1.u.f >= d2.u.f) ? 1 : 0;
		d1.type = INT;
	} else {
		d1.u.i = (d1.u.i >= d2.u.i) ? 1 : 0;
	}
	g_lingo->push(d1);
}

void LC::c_le() {
	Datum d2 = g_lingo->pop();
	Datum d1 = g_lingo->pop();

	if (g_lingo->alignTypes(d1, d2) == FLOAT) {
		d1.u.i = (d1.u.f <= d2.u.f) ? 1 : 0;
		d1.type = INT;
	} else {
		d1.u.i = (d1.u.i <= d2.u.i) ? 1 : 0;
	}
	g_lingo->push(d1);
}

void LC::c_jump() {
	uint jump = g_lingo->readInt();
	g_lingo->_pc = jump;
}

void LC::c_jumpifz() {
	uint jump = g_lingo->readInt();
	Datum test = g_lingo->pop();
	test.toInt();
	if (test.u.i == 0) {
		g_lingo->_pc = jump;
	}
}

void LC::c_repeatwhilecode(void) {
	Datum d;
	int savepc = g_lingo->_pc;

	uint body = g_lingo->getInt(savepc);
	uint end =  g_lingo->getInt(savepc + 1);

	g_lingo->execute(savepc + 2);	/* condition */
	d = g_lingo->pop();
	d.toInt();

	while (d.u.i) {
		g_lingo->execute(body + savepc - 1);	/* body */
		if (g_lingo->_returning)
			break;

		if (g_lingo->_exitRepeat) {
			g_lingo->_exitRepeat = false;
			break;
		}

		g_lingo->execute(savepc + 2);	/* condition */
		d = g_lingo->pop();
		d.toInt();
	}

	if (!g_lingo->_returning)
		g_lingo->_pc = end + savepc - 1; /* next stmt */
}

void LC::c_repeatwithcode(void) {
	Datum d;
	int savepc = g_lingo->_pc;

	uint init = g_lingo->getInt(savepc);
	uint finish =  g_lingo->getInt(savepc + 1);
	uint body = g_lingo->getInt(savepc + 2);
	int inc = (int32)g_lingo->getInt(savepc + 3);
	uint end = g_lingo->getInt(savepc + 4);
	Common::String countername((char *)&(*g_lingo->_currentScript)[savepc + 5]);
	Symbol *counter = g_lingo->lookupVar(countername.c_str());

	if (counter->type == CASTREF) {
		error("Cast ref used as index: %s", countername.c_str());
	}

	g_lingo->execute(init + savepc - 1);	/* condition */
	d = g_lingo->pop();
	d.toInt();
	counter->u.i = d.u.i;
	counter->type = INT;

	while (true) {
		g_lingo->execute(body + savepc - 1);	/* body */
		if (g_lingo->_returning)
			break;

		if (g_lingo->_exitRepeat) {
			g_lingo->_exitRepeat = false;
			break;
		}

		counter->u.i += inc;
		g_lingo->execute(finish + savepc - 1);	/* condition */
		d = g_lingo->pop();
		d.toInt();

		if (counter->u.i == d.u.i + inc)
			break;
	}

	if (!g_lingo->_returning)
		g_lingo->_pc = end + savepc - 1; /* next stmt */
}

void LC::c_exitRepeat(void) {
	g_lingo->_exitRepeat = true;
}

void LC::c_ifcode() {
	Datum d;
	int savepc = g_lingo->_pc;	/* then part */

	uint then =    g_lingo->getInt(savepc);
	uint elsep =   g_lingo->getInt(savepc+1);
	uint end =     g_lingo->getInt(savepc+2);
	uint skipEnd = g_lingo->getInt(savepc+3);

	debugC(8, kDebugLingoExec, "executing cond (have to %s end)", skipEnd ? "skip" : "execute");
	g_lingo->execute(savepc + 4);	/* condition */

	d = g_lingo->pop();

	if (d.toInt()) {
		debugC(8, kDebugLingoExec, "executing then");
		g_lingo->execute(then + savepc - 1);
	} else if (elsep) { /* else part? */
		debugC(8, kDebugLingoExec, "executing else");
		g_lingo->execute(elsep + savepc - 1);
	}

	// Since we do recursive calls, we want to skip behind end of the 'if'
	// statement only once, and not after every 'end if' call
	if (!g_lingo->_returning && !skipEnd) {
		g_lingo->_pc = end + savepc - 1; /* next stmt */
		debugC(8, kDebugLingoExec, "executing end");
	} else {
		debugC(8, kDebugLingoExec, "skipped end");
	}
}

void LC::c_whencode() {
	Datum d;
	uint start = g_lingo->_pc;
	uint end = g_lingo->readInt() + start - 1;
	Common::String eventname(g_lingo->readString());

	start = g_lingo->_pc;

	debugC(1, kDebugLingoExec, "c_whencode([%5d][%5d], %s)", start, end, eventname.c_str());

	int entity = g_lingo->_currentEntityId;
	g_lingo->_currentEntityId = 0;

	Symbol *sym = g_lingo->define(eventname, start, 0, NULL, end, false); // Redefine, but not remove code

	g_lingo->_currentEntityId = entity;

	if (debugChannelSet(1, kDebugLingoExec)) {
		uint pc = 0;
		while (pc < sym->u.defn->size()) {
			uint spc = pc;
			Common::String instr = g_lingo->decodeInstruction(sym->u.defn, pc, &pc);
			debugC(1, kDebugLingoExec, "[%5d] %s", spc, instr.c_str());
		}
	}

	g_lingo->_pc = end + 1;
}

void LC::c_tellcode() {
	warning("STUB: c_tellcode");
}

void LC::c_tell() {
	Datum d1 = g_lingo->pop();
	warning("STUB: c_tell %d", d1.u.i);
}

void LC::c_telldone() {
	warning("STUB: c_telldone");
}


//************************
// Built-in functions
//************************
void LC::c_goto() {
	Datum mode = g_lingo->pop();
	Datum frame, movie;

	if (mode.u.i == 2 || mode.u.i == 3)
		movie = g_lingo->pop();

	if (mode.u.i == 1 || mode.u.i == 3)
		frame = g_lingo->pop();

	g_lingo->func_goto(frame, movie);
}

void LC::c_gotoloop() {
	g_lingo->func_gotoloop();
}

void LC::c_gotonext() {
	g_lingo->func_gotonext();
}

void LC::c_gotoprevious() {
	g_lingo->func_gotoprevious();
}

void LC::c_play() {
	Datum mode = g_lingo->pop();
	Datum frame, movie;

	if (mode.u.i == 2 || mode.u.i == 3)
		movie = g_lingo->pop();

	if (mode.u.i == 1 || mode.u.i == 3)
		frame = g_lingo->pop();

	g_lingo->func_play(frame, movie);
}

void LC::c_playdone() {
	g_lingo->func_playdone();
}

void LC::c_call() {
	Common::String name(g_lingo->readString());

	int nargs = g_lingo->readInt();

	LC::call(name, nargs);
}

void LC::call(Common::String name, int nargs) {
	bool dropArgs = false;

	if (debugChannelSet(3, kDebugLingoExec))
		g_lingo->printSTUBWithArglist(name.c_str(), nargs, "call:");

	Symbol *sym = g_lingo->getHandler(name);

	if (!g_lingo->_eventHandlerTypeIds.contains(name)) {
		Symbol *s = g_lingo->lookupVar(name.c_str(), false);
		if (s && s->type == OBJECT) {
			debugC(3, kDebugLingoExec,  "Dereferencing object reference: %s to %s", name.c_str(), s->u.s->c_str());
			name = *s->u.s;
			sym = g_lingo->getHandler(name);
		}
	}

	if (sym == NULL) {
		warning("Call to undefined handler '%s'. Dropping %d stack items", name.c_str(), nargs);
		dropArgs = true;
	} else {
		if ((sym->type == BLTIN || sym->type == FBLTIN || sym->type == RBLTIN)
				&& sym->nargs != -1 && sym->nargs != nargs && sym->maxArgs != nargs) {
			if (sym->nargs == sym->maxArgs)
				warning("Incorrect number of arguments to handler '%s', expecting %d. Dropping %d stack items", name.c_str(), sym->nargs, nargs);
			else
				warning("Incorrect number of arguments to handler '%s', expecting %d or %d. Dropping %d stack items", name.c_str(), sym->nargs, sym->maxArgs, nargs);

			dropArgs = true;
		}
	}

	if (dropArgs) {
		for (int i = 0; i < nargs; i++)
			g_lingo->pop();

		// Push dummy value
		g_lingo->pushVoid();

		return;
	}

	if (sym->nargs != -1 && sym->maxArgs < nargs) {
		warning("Incorrect number of arguments for function %s (%d, expected %d to %d). Dropping extra %d",
					name.c_str(), nargs, sym->nargs, sym->maxArgs, nargs - sym->nargs);
		for (int i = 0; i < nargs - sym->maxArgs; i++)
			g_lingo->pop();
	}

	if (sym->type == BLTIN || sym->type == FBLTIN || sym->type == RBLTIN) {
		if (sym->u.bltin == LB::b_factory) {
			g_lingo->factoryCall(name, nargs);
		} else {
			int stackSize = g_lingo->_stack.size() - nargs;

			(*sym->u.bltin)(nargs);

			int stackNewSize = g_lingo->_stack.size();

			if (sym->type == FBLTIN || sym->type == RBLTIN) {
				if (stackNewSize - stackSize != 1)
					warning("built-in function %s did not return value", name.c_str());
			} else {
				if (stackNewSize - stackSize != 0)
					warning("built-in procedure %s returned extra %d values", name.c_str(), stackNewSize - stackSize);
			}
		}

		return;
	}

	for (int i = nargs; i < sym->nargs; i++) {
		Datum d;

		d.u.s = NULL;
		d.type = VOID;
		g_lingo->push(d);
	}

	debugC(5, kDebugLingoExec, "Pushing frame %d", g_lingo->_callstack.size() + 1);
	CFrame *fp = new CFrame;

	fp->sp = sym;
	fp->retpc = g_lingo->_pc;
	fp->retscript = g_lingo->_currentScript;
	fp->localvars = g_lingo->_localvars;

	// Create new set of local variables
	g_lingo->_localvars = new SymbolHash;

	g_lingo->_callstack.push_back(fp);

	g_lingo->_currentScript = sym->u.defn;
	g_lingo->execute(0);

	g_lingo->_returning = false;
}

void LC::c_procret() {
	if (!g_lingo->_callstack.size()) {
		warning("c_procret: Call stack underflow");
		g_lingo->_returning = true;
		return;
	}

	debugC(5, kDebugLingoExec, "Popping frame %d", g_lingo->_callstack.size() + 1);

	CFrame *fp = g_lingo->_callstack.back();
	g_lingo->_callstack.pop_back();

	g_lingo->_currentScript = fp->retscript;
	g_lingo->_pc = fp->retpc;

	g_lingo->cleanLocalVars();

	// Restore local variables
	g_lingo->_localvars = fp->localvars;

	delete fp;

	g_lingo->_returning = true;
}

void LC::c_global() {
	Common::String name(g_lingo->readString());

	Symbol *s = g_lingo->lookupVar(name.c_str(), false);
	if (s && !s->global) {
		warning("Local variable %s declared as global", name.c_str());
	}

	s = g_lingo->lookupVar(name.c_str(), true, true);
	s->global = true;
}

void LC::c_property() {
	Common::String name(g_lingo->readString());

	warning("STUB: c_property()");
}

void LC::c_instance() {
	Common::String name(g_lingo->readString());

	warning("STUB: c_instance(%s)", name.c_str());
}

void LC::c_factory() {
	Common::String name(g_lingo->readString());
	Datum d;

	warning("STUB: c_factory(%s)", name.c_str());

	d.type = OBJECT;
	d.u.s = new Common::String(name);

	g_lingo->push(d);
}

void LC::c_open() {
	Datum d2 = g_lingo->pop();
	Datum d1 = g_lingo->pop();

	d1.toString();
	d2.toString();

	warning("STUB: c_open(%s, %s)", d1.u.s->c_str(), d2.u.s->c_str());
}

void LC::c_hilite() {
	Datum first_char = g_lingo->pop();
	Datum last_char = g_lingo->pop();
	Datum first_word = g_lingo->pop();
	Datum last_word = g_lingo->pop();
	Datum first_item = g_lingo->pop();
	Datum last_item = g_lingo->pop();
	Datum first_line = g_lingo->pop();
	Datum last_line = g_lingo->pop();
	Datum cast_id = g_lingo->pop();

	warning("STUB: c_hilite: %d %d %d %d %d %d %d %d %d",
		first_char.u.i, last_char.u.i, first_word.u.i, last_word.u.i,
		first_item.u.i, last_item.u.i, first_line.u.i, last_line.u.i,
		cast_id.u.i);
}

void LC::c_unk() {
	uint opcode = g_lingo->readInt();
	warning("STUB: opcode 0x%02x", opcode);
}

void LC::c_unk1() {
	uint opcode = g_lingo->readInt();
	uint arg1 = g_lingo->readInt();
	warning("STUB: opcode 0x%02x (%d)", opcode, arg1);
}

void LC::c_unk2() {
	uint opcode = g_lingo->readInt();
	uint arg1 = g_lingo->readInt();
	uint arg2 = g_lingo->readInt();
	warning("STUB: opcode 0x%02x (%d, %d)", opcode, arg1, arg2);
}


}
