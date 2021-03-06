// Copyright 2016 Cheng Zhao. All rights reserved.
// Use of this source code is governed by the license that can be found in the
// LICENSE file.

#include "nativeui/window.h"

#include "nativeui/container.h"
#include "nativeui/menu_bar.h"
#include "third_party/yoga/yoga/Yoga.h"

#if defined(OS_MACOSX)
#include "nativeui/toolbar.h"
#endif

namespace nu {

Window::Window(const Options& options)
    : has_frame_(options.frame),
      transparent_(options.transparent),
      yoga_config_(YGConfigNew()) {
  // Initialize.
  PlatformInit(options);
  SetContentView(new Container);

  // Default window abilities.
  if (!options.frame && options.transparent) {
    SetResizable(false);
    SetMaximizable(false);
  }
}

Window::~Window() {
  PlatformDestroy();
}

void Window::SetContentView(View* view) {
  if (!view) {
    LOG(ERROR) << "Content view can not be null";
    return;
  }
  if (content_view_)
    content_view_->BecomeContentView(nullptr);
  PlatformSetContentView(view);
  content_view_ = view;
  content_view_->BecomeContentView(this);
}

View* Window::GetContentView() const {
  return content_view_.get();
}

SizeF Window::GetContentSize() const {
  return content_view_->GetBounds().size();
}

#if defined(OS_WIN) || defined(OS_LINUX)
void Window::SetMenuBar(MenuBar* menu_bar) {
  if (menu_bar_)
    menu_bar_->SetWindow(nullptr);
  PlatformSetMenuBar(menu_bar);
  menu_bar_ = menu_bar;
  menu_bar_->SetWindow(this);
}
#endif

}  // namespace nu
