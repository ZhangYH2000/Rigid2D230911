#pragma once
#define dt (cur.fps.dt())
#define dur (cur.fps.dur())
#define scr (cur.scr)
#define dscr (cur.dscr)
#define ft (cur.ft)
#define loc (cur.loc)

#define msw (cur.in.msw)
#define chs (cur.in.chs)
#define msp (cur.in.msp)
#define msp_old (cur.in.msp_old)
#define msd (cur.in.msd)
#define msd_old (cur.in.msd_old)
#define msnd (!msd[0] && !msd[1] && !msd[2])
#define msc (cur.in.msc)
#define kbmsg (cur.in.kbmsg)
#define kbd (cur.in.kbd)
#define kbd_old (cur.in.kbd_old)
#define kbc (cur.in.kbc)

#define own (cur.own)
#define hvd (own.hvd)
#define dhv (own.dhv)
#define whd (own.whd)
#define dwh (own.dwh)
#define kb (own.kb)
#define free_kb (own.free_kb)
#define rmv own.remove(this);
#define rhvd (hovered && msnd)

#define mkcl(x) mkp(x)(tmp); tmp = {};

#define ui (*cur.ui)
#define bgr (*cur.bgr)
#define gl (cur.gl)
#define dbstr (cur.dbstr)
