/*
 * Copyright 2015 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef NATIVE_AUDIO_AUDIO_RECORDER_H
#define NATIVE_AUDIO_AUDIO_RECORDER_H
#include <sys/types.h>
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
#include "audio_common.h"
#include "buf_manager.h"
#include "debug_utils.h"

class AudioRecorder {
  SLObjectItf recObjectItf_;
  SLRecordItf recItf_;
  SLAndroidSimpleBufferQueueItf recBufQueueItf_;

  SLAndroidEffectItf aecItf_;
  SLAndroidEffectItf nsItf_;

  SampleFormat sampleInfo_;
  AudioQueue *freeQueue_;       // user
  AudioQueue *recQueue_;        // user
  AudioQueue *devShadowQueue_;  // owner
  uint32_t audioBufCount;

  ENGINE_CALLBACK callback_;
  void *ctx_;

 public:
  explicit AudioRecorder(SampleFormat *, SLEngineItf engineEngine, SLObjectItf pItf_, bool aec, bool ns);
  ~AudioRecorder();
  SLboolean Start(void);
  SLboolean Stop(void);
  void SetBufQueues(AudioQueue *freeQ, AudioQueue *recQ);
  void ProcessSLCallback(SLAndroidSimpleBufferQueueItf bq);
  void RegisterCallback(ENGINE_CALLBACK cb, void *ctx);
  int32_t dbgGetDevBufCount(void);
  bool isAecEnabled();
  bool isNsEnabled();
  void setAecEnabled(bool enable);
  void setNsEnabled(bool enable);

  bool isAecSupported();
  bool isNsSupported();

#ifdef ENABLE_LOG
  AndroidLog *recLog_;
#endif

private:

    SLInterfaceID_ AEC_ = { 0x7b491460, 0x8d4d, 0x11e0, 0xbd61, { 0x00, 0x02, 0xa5, 0xd5, 0xc5, 0x1b} };
    SLInterfaceID_ NS_ = { 0x58b4b260, 0x8e06, 0x11e0, 0xaa8e, { 0x00, 0x02, 0xa5, 0xd5, 0xc5, 0x1b} };

    SLInterfaceID AEC = &AEC_;
    SLInterfaceID NS = &NS_;

};

#endif  // NATIVE_AUDIO_AUDIO_RECORDER_H
