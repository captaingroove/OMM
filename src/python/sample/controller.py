#!/usr/bin/env python3
#encoding: UTF-8

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

import time
import upnp

class TestController(upnp.Controller):
    # implement callback for device container pop up events
    def addDeviceContainer(self, device_container, index, begin):
        if not begin: print("TestController device container added, root device uuid: " + device_container.getRootDevice().getUuid())

    # implement callback for device container remove events
    def removeDeviceContainer(self, device_container, index, begin):
        if not begin: print("TestController device container removed, root device uuid: " + device_container.getRootDevice().getUuid())

server_group = upnp.CtlMediaServerGroup()
renderer_group = upnp.CtlMediaRendererGroup()
c = TestController()
# register Media Servers and Media Renderers (other devices will be ignored)
c.registerDeviceGroup(server_group)
c.registerDeviceGroup(renderer_group)
c.init()
# start the controller
c.setState(upnp.Controller.Public)

# wait for a Media Server to pop up
while server_group.getDeviceCount() == 0:
    time.sleep(0.5)

# get the first Media Server
server = server_group.getDevice(0)
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
        while renderer_group.getDeviceCount() == 0:
            time.sleep(0.5)

        # get the first Media Renderer
        renderer = renderer_group.getDevice(0)
        print("media renderer device friendly name: ", renderer.getFriendlyName())
        # set the AVTransportURI to the child object
        renderer.setObject(child, None, 0)
        # play the child object
        renderer.playPressed()

# stop the controller and leave the renderer playing the child object
c.setState(upnp.Controller.Stopped)
