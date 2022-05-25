#!/usr/bin/env python3
# -*- coding: utf-8 -*-

#***************************************************************************#
#  OMM - Open Multimedia                                                    #
#                                                                           #
#  Copyright (C) 2016                                                       #
#  Jörg Bakker (jb'at'open-multimedia.org)                                  #
#                                                                           #
#  This file is part of OMM.                                                #
#                                                                           #
#  OMM is free software: you can redistribute it and/or modify              #
#  it under the terms of the GNU General Public License as published by     #
#  the Free Software Foundation version 3 of the License.                   #
#                                                                           #
#  OMM is distributed in the hope that it will be useful,                   #
#  but WITHOUT ANY WARRANTY; without even the implied warranty of           #
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            #
#  GNU General Public License for more details.                             #
#                                                                           #
#  You should have received a copy of the GNU General Public License        #
#  along with this program.  If not, see <http://www.gnu.org/licenses/>.    #
#***************************************************************************#

# renderer dependencies:
# mplayer
# mplayer.py

import time
# import os

from prompt_toolkit import Application
# from prompt_toolkit.buffer import Buffer
from prompt_toolkit.layout.containers import HSplit, Window
from prompt_toolkit.layout.controls import UIContent, UIControl #, BufferControl, FormattedTextControl
# from prompt_toolkit.layout.controls import FormattedTextControl
# from prompt_toolkit.layout.controls import BufferControl
from prompt_toolkit.layout.layout import Layout
from prompt_toolkit.key_binding import KeyBindings

from prompt_toolkit.layout.screen import Point

# import mplayer
import os
import sys
# FIXME: hack to add the upnp python module
sys.path.append(os.path.join(os.environ.get("OMM") + "bin", "bindings"))
import upnp

class TestController(upnp.Controller):

    def __init__(self):
        super(TestController, self).__init__()
        window = Window(content=ListControl(self, kb_list))
        layout = Layout(window)
        # from the ptk reference manual:
        #
        # prompt_toolkit.application.get_app() → Application[Any]
        # Get the current active (running) Application. An Application is active during the Application.run_async() call.
        # We assume that there can only be one Application active at the same time.
        # There is only one terminal window, with only one stdin and stdout.
        # This makes the code significantly easier than passing around the Application everywhere.
        # If no Application is running, then return by default a DummyApplication.
        # For practical reasons, we prefer to not raise an exception.
        # This way, we don’t have to check all over the place whether an actual Application was returned.
        # (For applications like pymux where we can have more than one Application, we’ll use a work-around to handle that.)
        #
        # prompt_toolkit.application.set_app(app: Application[Any]) → Generator[None, None, None]
        # Context manager that sets the given Application active in an AppSession.
        # This should only be called by the Application itself.
        # The application will automatically be active while its running.
        # If you want the application to be active in other threads/coroutines, where that’s not the case,
        # use contextvars.copy_context(), or use Application.context to run it in the appropriate context.
        #
        #
        # NOTE: could also use get_app() to invalidate the Application from
        # outside of TestController
        self.app = Application(layout=layout, full_screen=True, key_bindings=kb_global)

    # implement callback for device container add events
    def addDeviceContainer(self, device_container, index, begin):
        if not begin:
            # print("TestController device container added, root device uuid: " +
            #                 device_container.getRootDevice().getUuid())
            self.app.invalidate()

    # implement callback for device container remove events
    def removeDeviceContainer(self, device_container, index, begin):
        if not begin:
            # print("TestController device container removed, root device uuid: " +
            #                 device_container.getRootDevice().getUuid())
            self.app.invalidate()

    def start(self):
        self.server_group = upnp.CtlMediaServerGroup()
        self.renderer_group = upnp.CtlMediaRendererGroup()
        # register Media Servers and Media Renderers (other devices will be ignored)
        self.registerDeviceGroup(self.server_group)
        self.registerDeviceGroup(self.renderer_group)
        self.init()
        # start the controller
        self.setState(upnp.Controller.Public)
        self.app.run()

    def stop(self):
        # stop the controller and leave the renderer playing the child object
        self.setState(upnp.Controller.Stopped)

    def play_first_title(self):
        # wait for a Media Server to pop up
        while self.server_group.getDeviceCount() == 0:
            time.sleep(0.5)

        # get the first Media Server
        server = self.server_group.getDevice(0)
        print("media server device friendly name: ", server.getFriendlyName())
        server.browseRootObject()
        root = server.getRootObject()
        print("root object title: ", root.getTitle())
        print("root object class: ", root.getClass())
        if upnp.AvClass.matchClass(root.getClass(),  upnp.AvClass.CONTAINER):
            print("root object child count: ", root.getChildCount())
            # get first child of root object
            child = root.getChildForRow(0)
            print("first child title: ", child.getTitle())
            print("first child object class: ", child.getClass())
            # check if child is an audio or video media object
            if upnp.AvClass.matchClass(child.getClass(), upnp.AvClass.ITEM, upnp.AvClass.AUDIO_ITEM) or \
            upnp.AvClass.matchClass(child.getClass(), upnp.AvClass.ITEM, upnp.AvClass.VIDEO_ITEM):
                res = child.getResource(0)
                print("first child resource uri: ", res.getUri())

                # wait for a Media Renderer to pop up
                while self.renderer_group.getDeviceCount() == 0:
                    time.sleep(0.5)

                # get the first Media Renderer
                renderer = self.renderer_group.getDevice(0)
                print("media renderer device friendly name: ", renderer.getFriendlyName())
                # set the AVTransportURI to the child object
                renderer.setObject(child, None, 0)
                # play the child object
                renderer.playPressed()

    def ls(self, path="/"):
        if path == "/":
            for server_idx in range(self.server_group.getDeviceCount()):
                server = self.server_group.getDevice(server_idx)
                print(server_idx, server.getFriendlyName() + "/")
        else:
            server_idx = int(path.lstrip("/"))
            server = self.server_group.getDevice(server_idx)
            server.browseRootObject()
            root = server.getRootObject()
            # print("root object title: ", root.getTitle())
            # print("root object class: ", root.getClass())
            if upnp.AvClass.matchClass(root.getClass(),  upnp.AvClass.CONTAINER):
                for child_idx in range(root.getChildCount()):
                    child = root.getChildForRow(child_idx)
                    if upnp.AvClass.matchClass(child.getClass(),
                                               upnp.AvClass.CONTAINER):
                        print(child_idx, child.getClass(), child.getTitle() + "/")
                    else:
                        print(child_idx, child.getClass(), child.getTitle())
                # check if child is an audio or video media object
                # if upnp.AvClass.matchClass(child.getClass(), upnp.AvClass.ITEM, upnp.AvClass.AUDIO_ITEM) or \
                # upnp.AvClass.matchClass(child.getClass(), upnp.AvClass.ITEM, upnp.AvClass.VIDEO_ITEM):

    def item(self, server_idx=0, child_idx=0):
        server = self.server_group.getDevice(server_idx)
        server.browseRootObject()
        root = server.getRootObject()
        return root.getChildForRow(child_idx)

    def uri(self, server_idx=0, child_idx=0):
        child = self.item(server_idx, child_idx)
        return child.getResource(0).getUri()

    def load_uri(self, server_idx=0, child_idx=0):
        self._uri = self.uri(server_idx, child_idx)

    # def play_uri(self, server_idx=0, child_idx=0):
    #     self._player = mplayer.Player()
    #     self._player.loadfile(self._uri)

    def pause_uri(self):
        self._player.pause()

    def stop_uri(self):
        self._player.quit()


kb_global = KeyBindings()

@kb_global.add('c-q')
def exit_(event):
    """
    Pressing Ctrl-Q will exit the user interface.

    Setting a return value means: quit the event loop that drives the user
    interface and return this value from the `Application.run()` call.
    """
    event.app.exit()


kb_list = KeyBindings()
cursor_position = Point(x=0, y=0)

@kb_list.add('down')
def key_down_(event):
    global cursor_position
    if cursor_position.y > 1:
        cursor_position = Point(cursor_position.x, cursor_position.y+1)


@kb_list.add('up')
def key_up_(event):
    global cursor_position
    cursor_position = Point(cursor_position.x, cursor_position.y-1)


@kb_list.add('r')
def key_refresh_(event):
    event.app.invalidate()


class ListControl(UIControl):

    def __init__(self,
                 controller,
                 key_bindings
                 ):
        self.controller = controller
        self.key_bindings = key_bindings

    def get_item_count(self):
        return self.controller.server_group.getDeviceCount()

    def get_item_count_str(self, server_idx):
        return str(self.get_item_count())

    def get_item(self, item_idx):
        device = self.controller.server_group.getDevice(item_idx)
        if device:
            return [("", device.getFriendlyName() + "/")]
        return None

    def create_content(self, width, height, preview_search=False):
        global cursor_position
        content = UIContent(
            get_line=self.get_item,
            line_count=self.get_item_count(),
            cursor_position=cursor_position)
        return content

    def get_key_bindings(self):
        return self.key_bindings


if __name__ == "__main__":
    upnp.Log.logToFile("/tmp/ommcontroller.log")

    controller = TestController()
    # c = TestController(app)
    # root_container = HSplit([
    #     Window(content=ListControl(c, kb_list)),
    # ])
    # layout = Layout(root_container)
    # window = Window(content=ListControl(controller, kb_list))
    # layout = Layout(window)
    # app = Application(layout=layout, full_screen=True, key_bindings=kb_global)

    controller.start()
    # app.run()
    controller.stop()
