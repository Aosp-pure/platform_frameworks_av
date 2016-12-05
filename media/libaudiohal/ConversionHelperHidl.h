/*
 * Copyright (C) 2016 The Android Open Source Project
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

#ifndef ANDROID_HARDWARE_CONVERSION_HELPER_HIDL_H
#define ANDROID_HARDWARE_CONVERSION_HELPER_HIDL_H

#include <android/hardware/audio/2.0/types.h>
#include <hidl/HidlSupport.h>
#include <utils/String8.h>

using ::android::hardware::audio::V2_0::ParameterValue;
using ::android::hardware::Return;
using ::android::hardware::Status;
using ::android::hardware::hidl_string;
using ::android::hardware::hidl_vec;

namespace android {

class ConversionHelperHidl {
  public:
    static void crashIfHalIsDead(const Status& status);

  protected:
    static status_t keysFromHal(const String8& keys, hidl_vec<hidl_string> *hidlKeys);
    static status_t parametersFromHal(const String8& kvPairs, hidl_vec<ParameterValue> *hidlParams);
    static void parametersToHal(const hidl_vec<ParameterValue>& parameters, String8 *values);

    ConversionHelperHidl(const char* className);

    status_t processReturn(const char* funcName, const Return<void>& ret) {
        return processReturn(funcName, ret.getStatus());
    }

    template<typename R, typename T>
    status_t processReturn(const char* funcName, const Return<R>& ret, T *retval) {
        if (ret.getStatus().isOk()) {
            // This way it also works for enum class to unscoped enum conversion.
            *retval = static_cast<T>(static_cast<R>(ret));
            return OK;
        }
        return processReturn(funcName, ret.getStatus());
    }

    status_t processReturn(const char* funcName, const Return<hardware::audio::V2_0::Result>& ret) {
        return processReturn(funcName, ret, ret);
    }

    template<typename T>
    status_t processReturn(
            const char* funcName, const Return<T>& ret, hardware::audio::V2_0::Result retval) {
        return processReturn(funcName, ret.getStatus(), retval);
    }

  private:
    const char* mClassName;

    static status_t analyzeResult(const hardware::audio::V2_0::Result& result);
    status_t processReturn(const char* funcName, const Status& status);
    status_t processReturn(
            const char* funcName, const Status& status, hardware::audio::V2_0::Result retval);
};

}  // namespace android

#endif // ANDROID_HARDWARE_CONVERSION_HELPER_HIDL_H
