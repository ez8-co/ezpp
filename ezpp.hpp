/*
  ezpp -- Easy performance profiler for C++.

  Copyright (c) 2010-2017 <http://ez8.co> <orca.zhang@yahoo.com>
  This library is released under the MIT License.
  Please see LICENSE file or visit https://github.com/ez8-co/ezpp for details.
 */
#pragma once

#include <typeinfo>
#include <string>
#include <map>
#include <vector>
#include <algorithm>

#include <thread>
#include <mutex>

#ifdef _WIN32
	#define int64_t __int64
	#define PRId64 "I64d"
#else
	#include <cstdint>
	#include <inttypes.h>
#endif

#define EZPP_ADD_OPTION(option)					::ezpp::inst().addOption(option)
#define EZPP_REMOVE_OPTION(option)				::ezpp::inst().removeOption(option)
#define EZPP_SET_OUTPUT_FILE_NAME(fileName)		::ezpp::inst().setOutputFileName(fileName)
#define EZPP_PRINT()							::ezpp::inst().print()
#define EZPP_SAVE(fileName)						::ezpp::inst().save(fileName)
#define EZPP_CLEAR()							::ezpp::inst().clear()
#define EZPP_ENABLED()							::ezpp::enabled()

#define EZPP_CUR_THREAD_ID						std::hash<std::thread::id>()(std::this_thread::get_id())

#define EZPP_SUB_CHECK(expression)				\
	do { \
		if(!::ezpp::enabled()) break; \
		expression; \
	} while(false)

////////////////////////////////////////////////////////////////////////////////////////////////////

#define EZPP_NODE_FLAG_IN_LOOP				0x08
#define EZPP_NODE_FLAG_DIRECT_OUTPUT        0x04
#define EZPP_NODE_FLAG_AUTO_START           0x02
#define EZPP_NODE_FLAG_CLS                  0x01

////////////////////////////////////////////////////////////////////////////////////////////////////

#define EZPP_OPT_SAVE_IN_DTOR               0x80

#define EZPP_OPT_SORT_BY_NAME      			0x40
#define EZPP_OPT_SORT_BY_CALL_CNT       	0x20
#define EZPP_OPT_SORT_BY_COST_TIME      	0x10

#define EZPP_OPT_FORCE_ENABLE               0x02
#define EZPP_OPT_FORCE_DISABLE              0x01

#define EZPP_OPT_EMPTY                      0x00

#define EZPP_OPT_SORT                  		(EZPP_OPT_SORT_BY_NAME | EZPP_OPT_SORT_BY_CALL_CNT | EZPP_OPT_SORT_BY_COST_TIME)
#define EZPP_OPT_SWITCH                    	(EZPP_OPT_FORCE_ENABLE | EZPP_OPT_FORCE_DISABLE)


#define EZPP_NODE_FUNC_DESC_EX(desc)		::ezpp::node_desc(__FILE__, __LINE__, __FUNCTION__, desc)

//////////////////////////////////////////////////////////////////////////

#define EZPP_AUX_BASE_SUB(sign, flags, desc)	\
	::ezpp::node_aux __ppn_a_##sign; \
	EZPP_SUB_CHECK(__ppn_a_##sign.set(::ezpp::ezpp::create(EZPP_NODE_FUNC_DESC_EX(desc), EZPP_CUR_THREAD_ID, EZPP_NODE_FLAG_AUTO_START | flags), EZPP_CUR_THREAD_ID))

#define EZPP()									EZPP_AUX_BASE_SUB(lc__, 0, "")
#define EZPP_IN_LOOP()							EZPP_AUX_BASE_SUB(lc_il__, EZPP_NODE_FLAG_IN_LOOP, "")
#define EZPP_EX(desc)							EZPP_AUX_BASE_SUB(ex_lc__, 0, desc)
#define EZPP_EX_IN_LOOP(desc)					EZPP_AUX_BASE_SUB(ex_lc_il__, EZPP_NODE_FLAG_IN_LOOP, desc)
#define EZPP_DO()								EZPP_AUX_BASE_SUB(lc_do__, EZPP_NODE_FLAG_DIRECT_OUTPUT, "")
#define EZPP_EX_DO(desc)						EZPP_AUX_BASE_SUB(ex_lc_do__, EZPP_NODE_FLAG_DIRECT_OUTPUT, desc)

//////////////////////////////////////////////////////////////////////////

#define EZPP_IL_DO_DECL_BASE(sign, flags, desc)			\
	::ezpp::node *__ppn_il_do_##sign##__ = 0; \
	EZPP_SUB_CHECK(__ppn_il_do_##sign##__ = ::ezpp::ezpp::create(EZPP_NODE_FUNC_DESC_EX(desc), EZPP_CUR_THREAD_ID, EZPP_NODE_FLAG_DIRECT_OUTPUT | flags))

#define EZPP_IL_DO_LIFE_CYCLE_CALL_BASE(sign)				\
	::ezpp::node_aux __ezpp_na_il_do_lc_##sign##__(__ppn_il_do_##sign##__, EZPP_CUR_THREAD_ID); \
	if(__ppn_il_do_##sign##__) { \
		__ppn_il_do_##sign##__->begin(EZPP_CUR_THREAD_ID); \
	}

#define EZPP_IL_DO_CODE_CLIP_CALL_BEGIN_BASE(sign)		\
	if(__ppn_il_do_##sign##__) { \
		__ppn_il_do_##sign##__->call(EZPP_CUR_THREAD_ID); \
	} \

#define EZPP_IL_DO_CODE_CLIP_CALL_END_BASE(sign)		\
	if(__ppn_il_do_##sign##__) { \
		__ppn_il_do_##sign##__->end(EZPP_CUR_THREAD_ID); \
	} \

#define EZPP_IL_DO_END_BASE(sign)						\
	if(__ppn_il_do_##sign##__) { \
		__ppn_il_do_##sign##__->end(EZPP_CUR_THREAD_ID); \
	} \

#define EZPP_IL_DO_DECL(sign)						EZPP_IL_DO_DECL_BASE(sign, 0, "")
#define EZPP_IL_DO_DECL_IL(sign)					EZPP_IL_DO_DECL_BASE(sign, EZPP_NODE_FLAG_IN_LOOP, "")
#define EZPP_IL_DO_LIFE_CYCLE_CALL(sign)			EZPP_IL_DO_LIFE_CYCLE_CALL_BASE(sign)
#define EZPP_IL_DO_CODE_CLIP_CALL_BEGIN(sign)		EZPP_IL_DO_CODE_CLIP_CALL_BEGIN_BASE(sign)
#define EZPP_IL_DO_CODE_CLIP_CALL_END(sign)			EZPP_IL_DO_CODE_CLIP_CALL_END_BASE(sign)
#define EZPP_IL_DO_END(sign)						EZPP_IL_DO_END_BASE(sign)

#define EZPP_IL_DO_EX_DECL(sign, desc)				EZPP_IL_DO_DECL_BASE(ex_##sign, 0, desc)
#define EZPP_IL_DO_EX_DECL_IL(sign, desc)			EZPP_IL_DO_DECL_BASE(ex_##sign, EZPP_NODE_FLAG_IN_LOOP, desc)
#define EZPP_IL_DO_EX_LIFE_CYCLE_CALL(sign)			EZPP_IL_DO_LIFE_CYCLE_CALL_BASE(ex_##sign)
#define EZPP_IL_DO_EX_CODE_CLIP_CALL_BEGIN(sign)	EZPP_IL_DO_CODE_CLIP_CALL_BEGIN_BASE(ex_##sign)
#define EZPP_IL_DO_EX_CODE_CLIP_CALL_END(sign)		EZPP_IL_DO_CODE_CLIP_CALL_END_BASE(ex_##sign)
#define EZPP_IL_DO_EX_END(sign)						EZPP_IL_DO_END_BASE(ex_##sign)

//////////////////////////////////////////////////////////////////////////

#define EZPP_NO_AUX_BEGIN_BASE(sign, flags, desc)	\
	::ezpp::node *__ppn_na_##sign##__ = 0; 			\
	EZPP_SUB_CHECK(__ppn_na_##sign##__ = ::ezpp::ezpp::create(EZPP_NODE_FUNC_DESC_EX(desc), EZPP_CUR_THREAD_ID, EZPP_NODE_FLAG_AUTO_START | flags))

#define EZPP_NO_AUX_END_BASE(sign)					\
	if(__ppn_na_##sign##__) {						\
		__ppn_na_##sign##__->setEndLineNum(__LINE__); \
		__ppn_na_##sign##__->end(EZPP_CUR_THREAD_ID); \
	} \

//////////////////////////////////////////////////////////////////////////

#define EZPP_CODE_CLIP_BEGIN(sign)					EZPP_NO_AUX_BEGIN_BASE(cc_##sign, 0, "")
#define EZPP_CODE_CLIP_END(sign)					EZPP_NO_AUX_END_BASE(cc_##sign)

#define EZPP_CODE_CLIP_BEGIN_EX(sign, desc)			EZPP_NO_AUX_BEGIN_BASE(cc_ex_##sign, 0, desc)
#define EZPP_CODE_CLIP_END_EX(sign)					EZPP_NO_AUX_END_BASE(cc_ex_##sign)

#define EZPP_CODE_CLIP_BEGIN_DO(sign)				EZPP_NO_AUX_BEGIN_BASE(cc_##sign, EZPP_NODE_FLAG_DIRECT_OUTPUT, "")
#define EZPP_CODE_CLIP_END_DO(sign)					EZPP_NO_AUX_END_BASE(cc_##sign)

#define EZPP_CODE_CLIP_BEGIN_EX_DO(sign, desc)		EZPP_NO_AUX_BEGIN_BASE(cc_ex_do_##sign, EZPP_NODE_FLAG_DIRECT_OUTPUT, desc)
#define EZPP_CODE_CLIP_END_EX_DO(sign)				EZPP_NO_AUX_END_BASE(cc_ex_do_##sign)

//////////////////////////////////////////////////////////////////////////

#define EZPP_CLS_REGISTER_BASE(sign)				\
	protected:										\
		::ezpp::node_aux __pp_na_cls_##sign;		\
	public:											\

#define EZPP_CLS_INIT_BASE(sign, flags, desc)		\
	EZPP_SUB_CHECK(__pp_na_cls_##sign.set(::ezpp::ezpp::create(::ezpp::ezpp::node_desc(__FILE__, __LINE__, typeid(*this).name(), desc), (int64_t)this, EZPP_NODE_FLAG_AUTO_START | EZPP_NODE_FLAG_CLS | flags), (int64_t)this))

#define EZPP_CLS_REGISTER()					EZPP_CLS_REGISTER_BASE(_)
#define EZPP_CLS_INIT()						EZPP_CLS_INIT_BASE(_, 0, "")

#define EZPP_CLS_REGISTER_EX()				EZPP_CLS_REGISTER_BASE(ex__)
#define EZPP_CLS_INIT_EX(desc)				EZPP_CLS_INIT_BASE(ex__, 0, desc)

#define EZPP_CLS_REGISTER_DO()				EZPP_CLS_REGISTER_BASE(do__)
#define EZPP_CLS_INIT_DO()					EZPP_CLS_INIT_BASE(do__, EZPP_NODE_FLAG_DIRECT_OUTPUT, "")

#define EZPP_CLS_REGISTER_EX_DO()			EZPP_CLS_REGISTER_BASE(ex_do__)
#define EZPP_CLS_INIT_EX_DO(desc)			EZPP_CLS_INIT_BASE(ex_do__, EZPP_NODE_FLAG_DIRECT_OUTPUT, desc)


namespace ezpp {

	#ifdef _WIN32
		#include <windows.h>
		namespace detail {
			LARGE_INTEGER init_freq() {
				LARGE_INTEGER freq;
				QueryPerformanceFrequency(&freq);
				return freq;
			}
		}
		int64_t time_now() {
			static LARGE_INTEGER freq = detail::init_freq();
			LARGE_INTEGER cnter;
			QueryPerformanceCounter(&cnter);
			return cnter.QuadPart / freq.QuadPart * 1000;
		}
	#else
		#include <sys/time.h>
		int64_t time_now() {
			timeval tv;
			gettimeofday(&tv, NULL);
			return tv.tv_usec / 1000 + tv.tv_sec * 1000;
		}
	#endif

	class node_desc
	{
	public:
		node_desc()
			: _fileName()
			, _line(0)
			, _endLine(0)
			, _cateName()
			, _extDesc()
		{
		}

		node_desc(const std::string& fileName,
					int lineNum,
					const std::string& cateName,
					const std::string& extDesc = "")
			: _fileName(fileName)
			, _line(lineNum)
			, _endLine(0)
			, _cateName(cateName)
			, _extDesc(extDesc)
		{
		}

		void outputFullDesc(FILE* fp) const
		{
			if(_line) {
				fprintf(fp, "%s(%s:%d", _cateName.c_str(), _fileName.c_str(), _line);
				if(_endLine) {
					fprintf(fp, "~%d", _endLine);
				}
				fprintf(fp, ")");
			}
			if(!_extDesc.empty()) {
				fprintf(fp, " \"%s\"", _extDesc.c_str());
			}
		}

		void setEndLineNum(int endLineNum) {
			_endLine = endLineNum;
		}

		bool operator<(const node_desc &other) const {
			if(_cateName != other._cateName)
				return _cateName < other._cateName;
			if(_fileName != other._fileName)
				return _fileName < other._fileName;
			if(_line != other._line)
				return _line < other._line;
			return _extDesc < other._extDesc;
		}

	private:
		std::string _fileName;
		int         _line;
		int         _endLine;
		std::string _cateName; // __FUNCTION__ \ typeid(*this).name()
		std::string _extDesc;
	}; // End class node_desc

	class node
	{
	public:
		friend class ezpp;

		inline int64_t getCallCnt() const
		{
			return _callCnt;
		}

		inline int64_t getCostTime() const
		{
			return _totalCostTime;
		}

		bool checkInUse()
		{
			return (_totalRefCnt > 0);
		}

		void setReleaseUntilEnd()
		{
			_releaseUntilEnd = true;
		}

		void begin(int64_t c12n);
		void end(int64_t c12n);
		void call(int64_t c12n);

		void setEndLineNum(int endLineNum) {
			_desc.setEndLineNum(endLineNum);
		}

		void output(FILE* fp);

	protected:
		node_desc _desc;

		unsigned char _flags;

		std::map<int64_t, int64_t> _beginTimeMap;
		std::map<int64_t, int64_t> _costTimeMap;

		int64_t _lastStartTime;
		int64_t _totalCostTime;

		int64_t _callCnt;

		std::map<int64_t, int64_t> _refMap;
		int64_t _totalRefCnt;

		int64_t _refCntRC;
		bool _releaseUntilEnd;

	private:
		explicit node(const node_desc& desc, int64_t c12n, unsigned char flags);

		std::mutex _lock;
	}; // End class node

	class node_aux
	{
	public:
		node_aux(node *ppNode = 0, int64_t c12n = 0)
			: _ppNode(ppNode)
			, _c12n(c12n) {
		}
		void set(node *ppNode, int64_t c12n) {
			_ppNode = ppNode;
			_c12n = c12n;
		}
		~node_aux() {
			if(_ppNode) {
				_ppNode->end(_c12n);
			}
		}

	private:
		node *_ppNode;
		int64_t _c12n;
	}; // End class node_aux

	class ezpp
	{
	public:

		static node* create(const node_desc& desc, int64_t c12n, unsigned char flags = EZPP_NODE_FLAG_AUTO_START);
		static void release(const std::pair<node_desc, node*>& node_pair);

		void addOption(unsigned char optModify);
		void removeOption(unsigned char optModify);

		void setOutputFileName(const std::string &fileName) {
			_fileName = fileName;
		}

		std::string getOutputFileName();

		void print();
		void save(const std::string& fileName = "");
		void clear();

	protected:
		ezpp();
		~ezpp();

	protected:
		friend class node;
		friend ezpp& inst();
		friend bool enabled();

		void removeDoNode(const node_desc& desc);

		void output(FILE* fp);
		static void outputTime(FILE* fp, int64_t duration);

		std::mutex _lockDoMap;
		std::map<node_desc, node *> _doMap;

		std::mutex _lockMap;
		std::map<node_desc, node *> _nodeMap;

		int64_t _beginTime;

		unsigned char _option;

		std::string _fileName;

		bool _enabled;
	};

	ezpp& inst() {
		static ezpp inst;
		return inst;
	}

	bool enabled() {
		return inst()._enabled;
	}

	static bool CallCntSort(node* left, node* right)
	{
		return left->getCallCnt() < right->getCallCnt();
	}

	static bool CostTimeSort(node* left, node* right)
	{
		return left->getCostTime() < right->getCostTime();
	}

	// protected
	void
	ezpp::outputTime(FILE* fp, int64_t duration)
	{
		double seconds = (double)duration / 1000;
		double minute = seconds / 60;
		double hour = minute / 60;

		if((int)hour > 0) {
			fprintf(fp, "%.0f hour%s, ", hour, hour > 1 ? "s" : "");
		}

		minute = (int64_t)minute % 60;
		if((int)minute > 0) {
			fprintf(fp, "%.0f min%s, ", minute, minute > 1 ? "s" : "");
		}

		seconds -= duration / 60 / 1000;
		if(seconds < 1) {
			fprintf(fp, "%2.2f ms", seconds * 1000);
		}
		else {
			fprintf(fp, "%2.2f sec%s", seconds, seconds > 1 ? "s" : "");
		}
	}

	// protected
	ezpp::ezpp()
		: _beginTime(0)
		, _option(EZPP_OPT_EMPTY)
		, _fileName()
		, _enabled(false)
	{
	}

	// protected
	ezpp::~ezpp()
	{
		print();
		if(_enabled && (_option & EZPP_OPT_SAVE_IN_DTOR)) {
			save();
		}
		clear();
	}

	// public static
	node* 
	ezpp::create(const node_desc& desc, int64_t c12n, unsigned char flags /*= EZPP_NODE_FLAG_AUTO_START*/)
	{
		if(!enabled() || !flags) {
			return 0;
		}
		std::mutex& mutex = (flags & EZPP_NODE_FLAG_DIRECT_OUTPUT) ? inst()._lockDoMap : inst()._lockMap;
		std::map<node_desc, node *>& map = (flags & EZPP_NODE_FLAG_DIRECT_OUTPUT) ? inst()._doMap : inst()._nodeMap;
		std::lock_guard<std::mutex> lock(mutex);
		std::map<node_desc, node *>::iterator it = map.find(desc);
		if(it != map.end()) {
			it->second->call(c12n);
			return it->second;
		}
		node* n = new node(desc, c12n, flags);
		map.insert(std::make_pair(desc, n));
		return n;
	}

	// public static
	void
	ezpp::release(const std::pair<node_desc, node*>& node_pair)
	{
		if(!node_pair.second->checkInUse()) {
			delete node_pair.second;
		}
		else {
			node_pair.second->setReleaseUntilEnd();
		}
	}

	void
	ezpp::output(FILE* fp)
	{
		std::lock_guard<std::mutex> lock(_lockMap);
		std::vector<node *> array;
		if(_nodeMap.size()) {
			bool store = (_option & EZPP_OPT_SORT & ~EZPP_OPT_SORT_BY_NAME);
			bool output = (_option & EZPP_OPT_SORT_BY_NAME) || !(_option & EZPP_OPT_SORT);
			fprintf(fp, "========== Easy Performance Profiler Report ==========\r\n");
			if(output) {
				fprintf(fp, "\r\n     [Sort By Name]\r\n\r\n");
			}
			size_t num = 0;
			for(std::map<node_desc, node *>::const_iterator it = _nodeMap.begin(); it != _nodeMap.end(); ++it) {
				if(store) {
					array.push_back(it->second);
				}
				if(output) {
					fprintf(fp, "No.%zd\r\n", ++num);
					it->second->output(fp);
				}
			}
			
			if(_option & EZPP_OPT_SORT_BY_CALL_CNT) {
				std::sort(array.begin(), array.end(), CallCntSort);
				fprintf(fp, "\r\n     [Sort By Call Count]\r\n\r\n");
				for(size_t i = 0; i < array.size(); ++i) {
					fprintf(fp, "No.%zd\r\n", i + 1);
					array[i]->output(fp);
				}
			}

			if(_option & EZPP_OPT_SORT_BY_COST_TIME) {
				std::sort(array.begin(), array.end(), CostTimeSort);
				fprintf(fp, "\r\n     [Sort By Cost Time]\r\n\r\n");
				for(size_t i = 0; i < array.size(); ++i) {
					fprintf(fp, "No.%zd\r\n", i + 1);
					array[i]->output(fp);
				}
			}
		}

		if(_nodeMap.size()) {
			fprintf(fp, "======[Total Time Elapsed] ");
			outputTime(fp, time_now() - _beginTime);
			time_t timep;
			time(&timep);
			char tmp[64];
			strftime(tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S", localtime(&timep));
			fprintf(fp, " ======\r\n======[Generate Date] %s ======\r\n", tmp);
		}
	}

	// public
	void 
	ezpp::print()
	{
		output(stdout);
	}

	// public
	std::string
	ezpp::getOutputFileName()
	{
		return _fileName.empty() ? "ezpp.log" : _fileName;
	}

	// public
	void 
	ezpp::save(const std::string& fileName/* = ""*/)
	{
		FILE* fp = fopen(fileName.empty() ? getOutputFileName().c_str() : fileName.c_str(), "wb+");
		output(fp);
		fclose(fp);
	}

	// public
	void 
	ezpp::clear()
	{
		{
			std::lock_guard<std::mutex> lock(_lockDoMap);
			std::for_each(_doMap.begin(), _doMap.end(), ezpp::release);
			_doMap.clear();
		}
		{
			std::lock_guard<std::mutex> lock(_lockMap);
			std::for_each(_nodeMap.begin(), _nodeMap.end(), ezpp::release);
			_nodeMap.clear();
		}
		_beginTime = time_now();
	}

	// protected
	void 
	ezpp::removeDoNode(const node_desc& desc)
	{
		std::lock_guard<std::mutex> lock(_lockDoMap);
		_doMap.erase(desc);
	}

	// public
	void 
	ezpp::addOption(unsigned char optModify)
	{
		if((optModify & EZPP_OPT_SWITCH) && (optModify & EZPP_OPT_SWITCH) != EZPP_OPT_SWITCH)
		{
			if((optModify & EZPP_OPT_FORCE_ENABLE) && !_enabled) {
				_enabled = true;
				_beginTime = time_now();
				_option &= ~EZPP_OPT_SWITCH;
				_option |= EZPP_OPT_FORCE_ENABLE;
			}
			if((optModify & EZPP_OPT_FORCE_DISABLE) && _enabled) {
				_enabled = false;
				_option &= ~EZPP_OPT_SWITCH;
				_option |= EZPP_OPT_FORCE_DISABLE;
			}
		}
		if(optModify & EZPP_OPT_SORT) {
			_option &= ~EZPP_OPT_SORT;
			_option |= (optModify & EZPP_OPT_SORT);
		}
		_option |= (optModify & EZPP_OPT_SAVE_IN_DTOR);
	}

	// public
	void 
	ezpp::removeOption(unsigned char optModify)
	{
		if((optModify & EZPP_OPT_FORCE_ENABLE) 
			&& (_option & EZPP_OPT_FORCE_ENABLE) 
			&& _enabled) {
			_enabled = false;
		}
		if((optModify & EZPP_OPT_FORCE_DISABLE) 
			&& (_option & EZPP_OPT_FORCE_DISABLE) 
			&& !_enabled) {
			_enabled = true;
			_beginTime = time_now();
		}
		_option &= ~optModify;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// node
	//

	// protected
	node::node(const node_desc& desc, int64_t c12n, unsigned char flags)
		: _desc(desc)
		, _flags(flags)
		, _lastStartTime(0)
		, _totalCostTime(0)
		, _callCnt(1)
		, _totalRefCnt(1)
		, _refCntRC(1)
		, _releaseUntilEnd(false)
	{
		if(_flags & EZPP_NODE_FLAG_AUTO_START) {
			begin(c12n);
		}
		else {
			++_totalRefCnt;
		}
		_lastStartTime = time_now();
	}

	// public
	void 
	node::call(int64_t c12n)
	{
		std::lock_guard<std::mutex> lock(_lock);

		int64_t now = time_now();
		if(!_refMap[EZPP_CUR_THREAD_ID]++ || (_flags & EZPP_NODE_FLAG_CLS)) {
			_beginTimeMap[c12n] = now;
		}
		if(_totalRefCnt == 0) {
			_lastStartTime = now;
		}
		++_totalRefCnt;
		++_callCnt;
	}

	// public
	void 
	node::begin(int64_t c12n)
	{
		if(!_beginTimeMap.empty()) {
			call(c12n);
			return;
		}
		_beginTimeMap.insert(std::make_pair(c12n, time_now()));
		_costTimeMap.insert(std::make_pair(c12n, 0));
		_refMap.insert(std::make_pair(EZPP_CUR_THREAD_ID, 1));
	}

	// public
	void 
	node::end(int64_t c12n)
	{
		bool delete_this = false;
		bool direct_output = false;
		{
			std::lock_guard<std::mutex> lock(_lock);

			--_totalRefCnt;
			int64_t now = time_now();
			if(!--_refMap[EZPP_CUR_THREAD_ID] || (_flags & EZPP_NODE_FLAG_CLS)) {
				_costTimeMap[c12n] += now - _beginTimeMap[c12n];
			}
			if(!_totalRefCnt) {
				_totalCostTime += now - _lastStartTime;

				if((_flags & EZPP_NODE_FLAG_DIRECT_OUTPUT)) {
					direct_output = true;
					delete_this = true;
				}
				else if(_releaseUntilEnd) {
					delete_this = true;
				}
			}
		}
		if(direct_output) {
			output(stdout);
			inst().removeDoNode(_desc);
		}
		if(delete_this) {
			delete this;
		}
	}

	// public
	void
	node::output(FILE* fp)
	{
		std::lock_guard<std::mutex> lock(_lock);
		fprintf(fp, "[Category] ");
		_desc.outputFullDesc(fp);
		fprintf(fp, "\r\n");
		if(_totalRefCnt) {
			fprintf(fp, "Warning: Unbalance detected! Mismatch or haven't been ended yet!\r\n");
		}
		fprintf(fp, "[Time] ");
		ezpp::outputTime(fp, _totalCostTime);
		if(_totalRefCnt) {
			fprintf(fp, " (+ ");
			ezpp::outputTime(fp, time_now() - _lastStartTime);
			fprintf(fp, ")");
		}
		size_t costTimeSize = _costTimeMap.size();
		if(costTimeSize == 1) {
			fprintf(fp, (_flags & EZPP_NODE_FLAG_CLS) ? "   (Object : 0x%0x)" : "   (Thread ID : %u)",
				(unsigned)_costTimeMap.begin()->first);
		}
		fprintf(fp, "\r\n");
		if(costTimeSize > 1) {
			int64_t total = 0;
			for(std::map<int64_t, int64_t>::iterator it = _costTimeMap.begin(); it != _costTimeMap.end(); ++it) {
				fprintf(fp, (_flags & EZPP_NODE_FLAG_CLS) ? "    (Object : 0x%0x) " :"    (Thread ID : %u) ", (unsigned)it->first);
				ezpp::outputTime(fp, it->second);
				fprintf(fp, "\r\n");
				total += it->second;
			}
			fprintf(fp, "  [Avg] ");
			ezpp::outputTime(fp, total / costTimeSize);
			fprintf(fp, "\r\n");
			fprintf(fp, "  [Total] ");
			ezpp::outputTime(fp, total);
			fprintf(fp, "\r\n");
		}
		fprintf(fp, "[Call] %" PRId64 "\r\n\r\n", _callCnt);
	}
}
