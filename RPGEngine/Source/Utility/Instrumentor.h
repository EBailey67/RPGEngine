/*
// Usage: include this header file somewhere in your code (eg. precompiled header), and then use like:
    
    Instrumentor::Get().BeginSession("Session Name");        // Begin session 
    {
        InstrumentationTimer timer("Profiled Scope Name");   // Place code like this in scopes you'd like to include in profiling
        // Code
    }
    Instrumentor::Get().EndSession();                        // End Session
    
    Use chrome://tracing or edge://tracing to load the output JSON file and view the timeline
    
*/
#pragma once

#include <string>
#include <chrono>
#include <algorithm>
#include <fstream>

#include <thread>


#if PROFILING
#define PROFILE_SCOPE(name) InstrumentationTimer timer##__LINE__(name)  // NOLINT(cppcoreguidelines-macro-usage)
#define PROFILE_FUNCTION() PROFILE_SCOPE(__FUNCSIG__)                   // NOLINT(cppcoreguidelines-macro-usage)
#else
#define PROFILE_SCOPE(name)
#define PROFILE_FUNCTION()
#endif


struct ProfileResult
{
    std::string name;
    long long start, end;
    size_t thread_id;
};

struct InstrumentationSession
{
    std::string Name;
};

class Instrumentor
{
private:
    InstrumentationSession* m_current_session;
    std::ofstream m_output_stream;
    int m_profile_count;

public:
    Instrumentor()
        : m_current_session(nullptr), m_profile_count(0)
    {
    }

    void BeginSession(const std::string& name, const std::string& filepath = "results.json")
    {
        m_output_stream.open(filepath);
        WriteHeader();
        m_current_session = new InstrumentationSession{ name };
    }

    void EndSession()
    {
        WriteFooter();
        m_output_stream.close();
        delete m_current_session;
        m_current_session = nullptr;
        m_profile_count = 0;
    }

    void WriteProfile(const ProfileResult& result)
    {
        if (m_profile_count++ > 0)
            m_output_stream << ",";

        std::string name = result.name;
        std::replace(name.begin(), name.end(), '"', '\'');

        m_output_stream << "{";
        m_output_stream << R"("cat":"function",)";
        m_output_stream << "\"dur\":" << (result.end - result.start) << ',';
        m_output_stream << R"("name":")" << name << "\",";
        m_output_stream << R"("ph":"X",)";
        m_output_stream << "\"pid\":0,";
        m_output_stream << "\"tid\":" << result.thread_id << ",";
        m_output_stream << "\"ts\":" << result.start;
        m_output_stream << "}";

        m_output_stream.flush();
    }

    void WriteHeader()
    {
        m_output_stream << R"({"otherData": {},"traceEvents":[)";
        m_output_stream.flush();
    }

    void WriteFooter()
    {
        m_output_stream << "]}";
        m_output_stream.flush();
    }

    static Instrumentor& Get()
    {
        static Instrumentor instance;
        return instance;
    }
};

class InstrumentationTimer
{
public:
    InstrumentationTimer(const char* name)
        : m_name(name), m_stopped(false)
    {
        m_start_timepoint = std::chrono::high_resolution_clock::now();
    }

    ~InstrumentationTimer()
    {
        if (!m_stopped)
            Stop();
    }

    void Stop()
    {
	    const auto endTimepoint = std::chrono::high_resolution_clock::now();

	    const auto start = std::chrono::time_point_cast<std::chrono::microseconds>(m_start_timepoint).time_since_epoch().count();
	    const auto end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

	    const auto threadID = std::hash<std::thread::id>{}(std::this_thread::get_id());
        Instrumentor::Get().WriteProfile({ m_name, start, end, threadID });

        m_stopped = true;
    }
	
private:
    const char* m_name;
    std::chrono::time_point<std::chrono::high_resolution_clock> m_start_timepoint;
    bool m_stopped;
};