/** @file dji_log.cpp
 *  @version 4.0.0
 *  @date Jun 15 2017
 *
 *  @brief
 *  Logging mechanism for printing status and error messages to the screen.
 *
 *  @Copyright (c) 2016-2017 DJI
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#include "dji_log.hpp"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

using namespace DJI::OSDK;

Log::Log(Mutex* m)
{
  if (m)
  {
    mutex = m;
    this->initFlag = true;
  }
  this->initFlag = false;
  this->enable_status = true;
  this->enable_debug  = false;
  this->enable_error = true;
  _func = [](char* ch){
    printf("%s\n", ch);
  };
}

Log::~Log()
{
  delete mutex;
}

Log&
Log::title(int level, const char* prefix, const char* func, int line)
{
  if(!initFlag)
  {
    mutex = new Mutex();
    initFlag = true;
  }

  if (level)
  {
    vaild = true;
    mutex->lock();
    uint32_t timeMs = 0;
    OsdkOsal_GetTimeMs(&timeMs);
    sprintf(buff, "[%d.%03d]%s/%d @ %s, L%d: ", timeMs / 1000, timeMs % 1000, prefix, level, func, line);
    if(_func)
      _func(buff);
    mutex->unlock();
  }
  else
  {
    vaild = false;
  }
  return *this;
}


Log&
Log::title(int level, const char* prefix)
{
  if(!initFlag)
  {
    mutex = new Mutex();
    initFlag = true;
  }

  if (level)
  {
    vaild = true;
    mutex->lock();
    sprintf(buff, "%s/%d" , prefix, level);
    _func(buff);
    mutex->unlock();
  }
  else
  {
    vaild = false;
  }
  return *this;
}

Log&
Log::print()
{
  return *this;
}

void Log::onMessage(const std::function<void (char*)>& func)
{
  _func = func;
}

Log&
Log::print(const char* fmt, ...)
{
  if(!initFlag)
  {
    mutex = new Mutex();
    initFlag = true;
  }
  if ((!release) && vaild)
  {
    va_list args;
    va_start(args, fmt);
    mutex->lock();
    vsnprintf(buff, sizeof(buff) - 1, fmt, args);
    va_end(args);
    if(buff[strlen(buff)] != '\n' && _func) {
      _func(buff);
      // printf("%s\n", log);
    } else if(_func) {
      _func(buff);
      // printf("%s", log);
    };
    mutex->unlock();
#if defined(__linux__)
      // fflush(stdout);
#endif
  }
  return *this;
}

Log&
Log::operator<<(bool val)
{
  if (val)
  {
    print("True");
  }
  else
  {
    print("False");
  }
  return *this;
}

Log&
Log::operator<<(short val)
{
  // @todo NUMBER_STYLE
  print("%d", val);
  return *this;
}

Log&
Log::operator<<(uint16_t val)
{
  // @todo NUMBER_STYLE
  print("%u", val);
  return *this;
}

Log&
Log::operator<<(int val)
{
  // @todo NUMBER_STYLE
  print("%d", val);
  return *this;
}

Log&
Log::operator<<(uint32_t val)
{
  // @todo NUMBER_STYLE
  print("%u", val);
  return *this;
}

Log&
Log::operator<<(long val)
{
  // @todo NUMBER_STYLE
  print("%ld", val);
  return *this;
}

Log&
Log::operator<<(unsigned long val)
{
  // @todo NUMBER_STYLE
  print("%lu", val);
  return *this;
}

Log&
Log::operator<<(long long val)
{
  // @todo NUMBER_STYLE
  print("%lld", val);
  return *this;
}

Log&
Log::operator<<(unsigned long long val)
{
  // @todo NUMBER_STYLE
  print("%llu", val);
  return *this;
}

Log&
Log::operator<<(float val)
{
  // @todo NUMBER_STYLE
  print("%f", val);
  return *this;
}

Log&
Log::operator<<(double val)
{
  // @todo NUMBER_STYLE
  print("%lf", val);
  return *this;
}

Log&
Log::operator<<(long double val)
{
  // @todo NUMBER_STYLE
  print("%Lf", val);
  return *this;
}

Log&
Log::operator<<(void* val)
{
  // @todo NUMBER_STYLE
  print("ptr:0x%X", val);
  return *this;
}

Log&
Log::operator<<(const char* str)
{
  print("%s", str);
  return *this;
}

Log&
Log::operator<<(char c)
{
  // @todo NUMBER_STYLE
  print("%c", c);
  return *this;
}

Log&
Log::operator<<(int8_t c)
{
  // @todo NUMBER_STYLE
  print("%c", c);
  return *this;
}

Log&
Log::operator<<(uint8_t c)
{
  // @todo NUMBER_STYLE
  print("0x%.2X", c);
  return *this;
}

// Various Toggles

void
Log::enableStatusLogging()
{
  this->enable_status = true;
}
void
Log::disableStatusLogging()
{
  this->enable_status = false;
}
void
Log::enableDebugLogging()
{
  this->enable_debug = true;
}
void
Log::disableDebugLogging()
{
  this->enable_debug = false;
}
void
Log::enableErrorLogging()
{
  this->enable_error = true;
}
void
Log::disableErrorLogging()
{
  this->enable_error = false;
}

bool
Log::getStatusLogState()
{
  return this->enable_status;
}

bool
Log::getDebugLogState()
{
  return this->enable_debug;
}

bool
Log::getErrorLogState()
{
  return this->enable_error;
}
