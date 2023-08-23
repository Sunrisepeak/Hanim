// Use of this source code is governed by Apache-2.0 License
// that can be found in the License file.
//
// Copyright (C) 2023 - present  Sunrisepeak
//
// Author: Sunrisepeak (speakshen@163.com)
// ProjectLinks: https://github.com/Sunrisepeak/HAnim
//


#ifndef __OBJECT_HPP__HANIM
#define __OBJECT_HPP__HANIM

#include <Hanim.hpp>

#ifdef HOBJECT_IMGUI
#include <hobject/HanimObject.imgui.hpp>
#endif

#ifdef HOBJECT_OPENGL
#include <hobject/HanimObject.opengl.hpp>
#endif

#ifdef HOBJECT_QT
#include <hobject/HanimObject.qt.hpp>
#endif

#endif