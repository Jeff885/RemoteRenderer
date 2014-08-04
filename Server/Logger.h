// 日志类头文件
// author：胡山峰
// date：2013-8-29

#ifndef _LOGGER_H
#define _LOGGER_H

#include "Global.h"
#include <string>

using namespace std;


	// 日志级别
	enum LogLevel {
		VERBOSE_LEVEL,
		DEBUG_LEVEL,
		INFO_LEVEL,
		WARNING_LEVEL,
		ERROR_LEVEL,
		FATAL_LEVEL,
	};

	//日志记录
	struct LogRecord {
		size_t _log_level; // 日志级别
		string _log_tag;     // 日志标题
		string _log_msg;     // 日志内容
	};

	// 日志抽象类，所有日志类继承该类，实现自定义的日志记录和显示方法
	class Logger {

	public:
		virtual  void doLog(LogRecord &log) = 0;

		  void LOG(
			const size_t log_level, 
			const string &log_tag, 
			const string &log_msg){
			LogRecord log;
			log._log_level = log_level;
			log._log_tag = log_tag;
			log._log_msg = log_msg;
			doLog(log);
		}

		  void VERBOSE( 
			const string &log_tag, 
			const string &log_msg){
				LOG(VERBOSE_LEVEL, log_tag, log_msg);
		}

		  void DEBUG(
			const string &log_tag, 
			const string &log_msg){
				LOG( DEBUG_LEVEL, log_tag, log_msg);
		}

		  void INFO(
			const string &log_tag, 
			const string &log_msg){
				LOG( INFO_LEVEL, log_tag, log_msg);
		}

		  void WARN( 
			const string &log_tag, 
			const string &log_msg){
				LOG( WARNING_LEVEL, log_tag, log_msg);
		}

		  void ERR(
			const string &log_tag, 
			const string &log_msg){
				LOG( ERROR_LEVEL, log_tag, log_msg);
		}

		  void FATAL(
			const string &log_tag, 
			const string &log_msg){
				LOG( FATAL_LEVEL, log_tag, log_msg);
		}
	};

	// 控制台日志类，直接将日志输出到控制台
	// 格式：xxxx/xx/xx xx:xx:xx    level    tag    msg
	class ConsoleLogger : public Logger {

	public:
		ConsoleLogger() {
			// do nothing
		}

		 virtual void doLog(LogRecord &log) {

			using namespace boost::gregorian;
			using namespace boost::posix_time;

			// 日期时间
			ptime now = second_clock::local_time();
			cout << now << "    ";

			// 级别
			switch(log._log_level){
			case VERBOSE_LEVEL: cout << "verbose    "; break;
			case DEBUG_LEVEL:   cout << "debug      "; break;
			case INFO_LEVEL:    cout << "info       "; break;
			case WARNING_LEVEL: cout << "warning    "; break;
			case ERROR_LEVEL:   cout << "error      "; break;
			case FATAL_LEVEL:   cout << "fatal      "; break;
			default:			cout << "default    "; break;
			}

			// tag和msg
			cout << log._log_tag << "		";
			cout << log._log_msg << endl;
		}
		 

		~ConsoleLogger() {
			// do nothing
			
		}
	};


#endif