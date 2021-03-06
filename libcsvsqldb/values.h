//
//  values.h
//  csvsqldb
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

#ifndef csvsqldb_values_h
#define csvsqldb_values_h

#include "libcsvsqldb/inc.h"

#include "types.h"

#include "base/float_helper.h"
#include "base/hash_helper.h"

#include <cstring>
#include <iomanip>


namespace csvsqldb
{

    class CSVSQLDB_EXPORT Value
    {
    public:
        virtual ~Value()
        {
        }

        virtual void toStream(std::ostream& stream) const = 0;

        virtual std::string toString() const = 0;

        virtual bool isNull() const = 0;

        virtual size_t size() const = 0;

        virtual eType getType() const = 0;

        virtual bool operator<(const Value& rhs) const = 0;

        virtual bool operator==(const Value& rhs) const = 0;

        virtual size_t getHash() const = 0;
    };


    class CSVSQLDB_EXPORT ValInt : public Value
    {
    public:
        ValInt()
        : _val(std::numeric_limits<int64_t>::max())
        , _isNull(true)
        {
        }

        ValInt(int64_t val)
        : _val(val)
        , _isNull(false)
        {
        }

        ValInt(const ValInt& rhs)
        : _val(rhs._val)
        , _isNull(rhs._isNull)
        {
        }

        virtual bool isNull() const
        {
            return _isNull;
        }

        int64_t asInt() const
        {
            return _val;
        }

        virtual void toStream(std::ostream& stream) const
        {
            if(!_isNull) {
                stream << _val;
            } else {
                stream << "NULL";
            }
        }

        virtual std::string toString() const
        {
            return std::to_string(_val);
        }

        static size_t baseSize()
        {
            return sizeof(ValInt);
        }

        virtual size_t size() const
        {
            return sizeof(ValInt);
        }

        virtual eType getType() const
        {
            return INT;
        }

        virtual bool operator<(const Value& rhs) const
        {
            if(isNull() || rhs.isNull()) {
                return false;
            }
            return _val < dynamic_cast<const ValInt&>(rhs)._val;
        }

        virtual bool operator==(const Value& rhs) const
        {
            if(isNull() || rhs.isNull()) {
                return false;
            }
            return _val == dynamic_cast<const ValInt&>(rhs)._val;
        }

        virtual size_t getHash() const
        {
            return std::hash<int64_t>()(_val);
        }

    private:
        int64_t _val;
        bool _isNull;
    } __attribute__((__packed__));


    class CSVSQLDB_EXPORT ValDouble : public Value
    {
    public:
        ValDouble()
        : _val(std::numeric_limits<double>::max())
        , _isNull(true)
        {
        }

        ValDouble(double val)
        : _val(val)
        , _isNull(false)
        {
        }

        ValDouble(const ValDouble& rhs)
        : _val(rhs._val)
        , _isNull(rhs._isNull)
        {
        }

        virtual bool isNull() const
        {
            return _isNull;
        }

        double asDouble() const
        {
            return _val;
        }

        virtual void toStream(std::ostream& stream) const
        {
            if(!_isNull) {
                stream << std::fixed << std::showpoint << std::setprecision(6) << _val;
            } else {
                stream << "NULL";
            }
        }

        virtual std::string toString() const
        {
            return std::to_string(_val);
        }

        static size_t baseSize()
        {
            return sizeof(ValDouble);
        }

        virtual size_t size() const
        {
            return sizeof(ValDouble);
        }

        virtual eType getType() const
        {
            return REAL;
        }

        virtual bool operator<(const Value& rhs) const
        {
            if(isNull() || rhs.isNull()) {
                return false;
            }
            return _val < dynamic_cast<const ValDouble&>(rhs)._val;
        }

        virtual bool operator==(const Value& rhs) const
        {
            if(isNull() || rhs.isNull()) {
                return false;
            }
            return csvsqldb::compare(_val, dynamic_cast<const ValDouble&>(rhs)._val);
        }

        virtual size_t getHash() const
        {
            return std::hash<double>()(_val);
        }

    private:
        double _val;
        bool _isNull;
    } __attribute__((__packed__));


    class CSVSQLDB_EXPORT ValBool : public Value
    {
    public:
        ValBool()
        : _val(false)
        , _isNull(true)
        {
        }

        ValBool(bool val)
        : _val(val)
        , _isNull(false)
        {
        }

        ValBool(const ValBool& rhs)
        : _val(rhs._val)
        , _isNull(rhs._isNull)
        {
        }

        virtual bool isNull() const
        {
            return _isNull;
        }

        bool asBool() const
        {
            return _val;
        }

        virtual void toStream(std::ostream& stream) const
        {
            if(!_isNull) {
                stream << _val;
            } else {
                stream << "NULL";
            }
        }

        virtual std::string toString() const
        {
            return std::to_string(_val);
        }

        static size_t baseSize()
        {
            return sizeof(ValBool);
        }

        virtual size_t size() const
        {
            return sizeof(ValBool);
        }

        virtual eType getType() const
        {
            return BOOLEAN;
        }

        virtual bool operator<(const Value& rhs) const
        {
            if(isNull() || rhs.isNull()) {
                return false;
            }
            return _val < dynamic_cast<const ValBool&>(rhs)._val;
        }

        virtual bool operator==(const Value& rhs) const
        {
            if(isNull() || rhs.isNull()) {
                return false;
            }
            return _val == dynamic_cast<const ValBool&>(rhs)._val;
        }

        virtual size_t getHash() const
        {
            return std::hash<bool>()(_val);
        }

    private:
        bool _val;
        bool _isNull;
    } __attribute__((__packed__));


    class CSVSQLDB_EXPORT ValDate : public Value
    {
    public:
        ValDate()
        : _isNull(true)
        {
        }

        ValDate(const csvsqldb::Date& val)
        : _val(val)
        , _isNull(false)
        {
        }

        virtual bool isNull() const
        {
            return _isNull;
        }

        const csvsqldb::Date& asDate() const
        {
            return _val;
        }

        virtual void toStream(std::ostream& stream) const
        {
            if(!_isNull) {
                stream << _val.format("%F");
            } else {
                stream << "NULL";
            }
        }

        virtual std::string toString() const
        {
            return _val.format("%F");
        }

        static size_t baseSize()
        {
            return sizeof(ValDate);
        }

        virtual size_t size() const
        {
            return sizeof(ValDate);
        }

        virtual eType getType() const
        {
            return DATE;
        }

        virtual bool operator<(const Value& rhs) const
        {
            if(isNull() || rhs.isNull()) {
                return false;
            }
            return _val < dynamic_cast<const ValDate&>(rhs)._val;
        }

        virtual bool operator==(const Value& rhs) const
        {
            if(isNull() || rhs.isNull()) {
                return false;
            }
            return _val == dynamic_cast<const ValDate&>(rhs)._val;
        }

        virtual size_t getHash() const
        {
            return std::hash<int64_t>()(_val.asJulianDay());
        }

    private:
        csvsqldb::Date _val;
        bool _isNull;
    } __attribute__((__packed__));


    class CSVSQLDB_EXPORT ValTime : public Value
    {
    public:
        ValTime()
        : _isNull(true)
        {
        }

        ValTime(const csvsqldb::Time& val)
        : _val(val)
        , _isNull(false)
        {
        }

        virtual bool isNull() const
        {
            return _isNull;
        }

        const csvsqldb::Time& asTime() const
        {
            return _val;
        }

        virtual void toStream(std::ostream& stream) const
        {
            if(!_isNull) {
                stream << _val.format("%H:%M:%S");
            } else {
                stream << "NULL";
            }
        }

        virtual std::string toString() const
        {
            return _val.format("%H:%M:%S");
        }

        static size_t baseSize()
        {
            return sizeof(ValTime);
        }

        virtual size_t size() const
        {
            return sizeof(ValTime);
        }

        virtual eType getType() const
        {
            return TIME;
        }

        virtual bool operator<(const Value& rhs) const
        {
            if(isNull() || rhs.isNull()) {
                return false;
            }
            return _val < dynamic_cast<const ValTime&>(rhs)._val;
        }

        virtual bool operator==(const Value& rhs) const
        {
            if(isNull() || rhs.isNull()) {
                return false;
            }
            return _val == dynamic_cast<const ValTime&>(rhs)._val;
        }

        virtual size_t getHash() const
        {
            return std::hash<int32_t>()(_val.asInteger());
        }

    private:
        csvsqldb::Time _val;
        bool _isNull;
    } __attribute__((__packed__));


    class CSVSQLDB_EXPORT ValTimestamp : public Value
    {
    public:
        ValTimestamp()
        : _isNull(true)
        {
        }

        ValTimestamp(const csvsqldb::Timestamp& val)
        : _val(val)
        , _isNull(false)
        {
        }

        virtual bool isNull() const
        {
            return _isNull;
        }

        const csvsqldb::Timestamp& asTimestamp() const
        {
            return _val;
        }

        virtual void toStream(std::ostream& stream) const
        {
            if(!_isNull) {
                stream << _val.format("%Y-%m-%dT%H:%M:%S");
            } else {
                stream << "NULL";
            }
        }

        virtual std::string toString() const
        {
            return _val.format("%Y-%m-%dT%H:%M:%S");
        }

        static size_t baseSize()
        {
            return sizeof(ValTimestamp);
        }

        virtual size_t size() const
        {
            return sizeof(ValTimestamp);
        }

        virtual eType getType() const
        {
            return TIMESTAMP;
        }

        virtual bool operator<(const Value& rhs) const
        {
            if(isNull() || rhs.isNull()) {
                return false;
            }
            return _val < dynamic_cast<const ValTimestamp&>(rhs)._val;
        }

        virtual bool operator==(const Value& rhs) const
        {
            if(isNull() || rhs.isNull()) {
                return false;
            }
            return _val == dynamic_cast<const ValTimestamp&>(rhs)._val;
        }

        virtual size_t getHash() const
        {
            return std::hash<int64_t>()(_val.asInteger());
        }

    private:
        csvsqldb::Timestamp _val;
        bool _isNull;
    } __attribute__((__packed__));


    class CSVSQLDB_EXPORT ValString : public Value
    {
    public:
        ValString()
        : _length(0)
        , _val(nullptr)
        {
        }

        ValString(const char* val)
        : _length(::strlen(val))
        , _val(val)
        {
        }

        ValString(const char* val, size_t len)
        : _length(len)
        , _val(val)
        {
        }

        virtual bool isNull() const
        {
            return _val == nullptr;
        }

        virtual ~ValString()
        {
            delete[] _val;
        }

        const char* asString() const
        {
            return _val;
        }

        virtual void toStream(std::ostream& stream) const
        {
            if(_val) {
                stream << _val;
            } else {
                stream << "NULL";
            }
        }

        virtual std::string toString() const
        {
            return _val;
        }

        static size_t baseSize()
        {
            return sizeof(ValString);
        }

        virtual size_t size() const
        {
            return sizeof(ValString) + _length + 1;
        }

        virtual eType getType() const
        {
            return STRING;
        }

        size_t length() const
        {
            return _length;
        }

        virtual bool operator<(const Value& rhs) const
        {
            if(isNull() || rhs.isNull()) {
                return false;
            }
            return ::strcoll(_val, dynamic_cast<const ValString&>(rhs)._val) < 0;
        }

        virtual bool operator==(const Value& rhs) const
        {
            if(isNull() || rhs.isNull()) {
                return false;
            }
            return ::strcoll(_val, dynamic_cast<const ValString&>(rhs)._val) == 0;
        }

        virtual size_t getHash() const
        {
            return std::hash<const char*>()(_val);
        }

    private:
        size_t _length;
        const char* _val;
    } __attribute__((__packed__));


    CSVSQLDB_EXPORT Value* createValue(eType type, const csvsqldb::Any& value);

    typedef std::vector<const Value*> Values;

    template <typename T>
    struct ValueCreator {
        static Value* createValue(const T& val);
    };

    template <>
    struct ValueCreator<bool> {
        static Value* createValue(const bool& val)
        {
            return new ValBool(val);
        }
    };

    template <>
    struct ValueCreator<int64_t> {
        static Value* createValue(const int64_t& val)
        {
            return new ValInt(val);
        }
    };

    template <>
    struct ValueCreator<double> {
        static Value* createValue(const double& val)
        {
            return new ValDouble(val);
        }
    };

    template <>
    struct ValueCreator<csvsqldb::Date> {
        static Value* createValue(const csvsqldb::Date& val)
        {
            return new ValDate(val);
        }
    };

    template <>
    struct ValueCreator<csvsqldb::Time> {
        static Value* createValue(const csvsqldb::Time& val)
        {
            return new ValTime(val);
        }
    };

    template <>
    struct ValueCreator<csvsqldb::Timestamp> {
        static Value* createValue(const csvsqldb::Timestamp& val)
        {
            return new ValTimestamp(val);
        }
    };

    template <>
    struct ValueCreator<std::string> {
        static Value* createValue(const std::string& val)
        {
            char* c = new char[val.length() + 1];
            std::size_t length = val.copy(&c[0], val.length());
            c[length] = '\0';
            return new ValString(c);
        }
    };

    template <>
    struct ValueCreator<StringType> {
        static Value* createValue(const StringType& val)
        {
            return new ValString(val);
        }
    };
}

namespace std
{
    template <>
    struct hash<csvsqldb::Value> {
        typedef csvsqldb::Value argument_type;
        typedef std::size_t result_type;

        result_type operator()(argument_type const& val) const
        {
            return val.getHash();
        }
    };
}

#endif
