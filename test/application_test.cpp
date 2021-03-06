//
//  csvsqldb test
//
//  BSD 3-Clause License
//  Copyright (c) 2015, Lars-Christian Fürstenberg
//  All rights reserved.
//
//  Redistribution and use in source and binary forms, with or without modification, are permitted
//  provided that the following conditions are met:
//
//  1. Redistributions of source code must retain the above copyright notice, this list of
//  conditions and the following disclaimer.
//
//  2. Redistributions in binary form must reproduce the above copyright notice, this list of
//  conditions and the following disclaimer in the documentation and/or other materials provided
//  with the distribution.
//
//  3. Neither the name of the copyright holder nor the names of its contributors may be used to
//  endorse or promote products derived from this software without specific prior written
//  permission.
//
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
//  IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
//  AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
//  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
//  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
//  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
//  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
//  OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
//  POSSIBILITY OF SUCH DAMAGE.
//


#include "test.h"

#include "libcsvsqldb/base/application.h"


class MyApplication : public csvsqldb::Application
{
public:
    MyApplication(int argc, char** argv, bool setUp)
    : csvsqldb::Application(argc, argv)
    , _didSetup(false)
    , _didRun(false)
    , _didTearDown(false)
    , _setUp(setUp)
    {
    }

    bool _didSetup;
    bool _didRun;
    bool _didTearDown;

private:
    virtual bool setUp(int argc, char** argv)
    {
        _didSetup = true;
        return _setUp;
    }

    virtual int doRun()
    {
        _didRun = true;
        return 0;
    }

    virtual void tearDown()
    {
        _didTearDown = true;
    }

    bool _setUp;
};


class ApplicationTestCase
{
public:
    void setUp()
    {
    }

    void tearDown()
    {
    }

    void run()
    {
        int argc = 2;
        const char* args[] = { "MyApp", "Arg1" };

        MyApplication myapp(argc, const_cast<char**>(args), true);
        int ret = myapp.run();

        MPF_TEST_ASSERTEQUAL(0, ret);
        MPF_TEST_ASSERTEQUAL(true, myapp._didSetup);
        MPF_TEST_ASSERTEQUAL(true, myapp._didRun);
        MPF_TEST_ASSERTEQUAL(true, myapp._didTearDown);
    }

    void dontRun()
    {
        int argc = 2;
        const char* args[] = { "MyApp", "Arg1" };

        MyApplication myapp(argc, const_cast<char**>(args), false);
        int ret = myapp.run();

        MPF_TEST_ASSERTEQUAL(-1, ret);
        MPF_TEST_ASSERTEQUAL(true, myapp._didSetup);
        MPF_TEST_ASSERTEQUAL(false, myapp._didRun);
        MPF_TEST_ASSERTEQUAL(false, myapp._didTearDown);
    }
};

MPF_REGISTER_TEST_START("ApplicationTestSuite", ApplicationTestCase);
MPF_REGISTER_TEST(ApplicationTestCase::run);
MPF_REGISTER_TEST(ApplicationTestCase::dontRun);
MPF_REGISTER_TEST_END();
