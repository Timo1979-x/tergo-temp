// Copyright 2022 Yoichiro Tanaka (@yoichiro)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#define HAL_USE_SIO TRUE
#define HAL_USE_I2C TRUE

// Для того, чтобы скомпилировалась поддержка обычной подсветки с использованием аппаратного PWM:
#define HAL_USE_PWM TRUE

#include_next <halconf.h>
