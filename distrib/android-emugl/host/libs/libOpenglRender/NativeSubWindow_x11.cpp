/*
* Copyright (C) 2011 The Android Open Source Project
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/
#include "NativeSubWindow.h"

#include <stdio.h>

static Bool WaitForMapNotify(Display *d, XEvent *e, char *arg) {
    if (e->type == MapNotify && e->xmap.window == (Window)arg) {
        return 1;
    }
    return 0;
}

static Display *s_display = NULL;

EGLNativeWindowType createSubWindow(FBNativeWindowType p_window,
                                    int x,
                                    int y,
                                    int width,
                                    int height) {
   // The call to this function is protected by a lock
   // in FrameBuffer so it is safe to check and initialize s_display here
   if (!s_display) {
       s_display = XOpenDisplay(NULL);
   }

    XSetWindowAttributes wa;
    wa.event_mask = StructureNotifyMask;
    Window win = XCreateWindow(s_display,
                               p_window,
                               x,
                               y,
                               width,
                               height,
                               0,
                               CopyFromParent,
                               CopyFromParent,
                               CopyFromParent,
                               CWEventMask,
                               &wa);
    XMapWindow(s_display,win);
    XEvent e;
    XIfEvent(s_display, &e, WaitForMapNotify, (char *)win);
    return win;
}

void destroySubWindow(EGLNativeWindowType win) {
    if (!s_display) {
        return;
    }
    XDestroyWindow(s_display, win);
}
