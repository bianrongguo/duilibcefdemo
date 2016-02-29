/** 
 * \file SPLogger.h
 * \brief 公用日志类头文件定义
 *
 * 用于系统的日志输出
 */

//#pragma once
#ifndef _SPLOGGER_H
#define _SPLOGGER_H

#include <stdlib.h>
#include <tchar.h>
#include <map>
#include <wtypes.h>


///////////////////////////////////////////////////////
//日志处理器定义
///////////////////////////////////////////////////////

//本地数据访问模块日志名称
#define LOG_SPDAMGR         _T("spdamgr")
//文件过滤钩子
#define LOG_SPFHK		 	_T("spfhk")
//文件过滤钩子
#define LOG_SPFLT		 	_T("spfflt")
//网络控制接
#define LOG_SPNETFLT	 	_T("spnetflt")
//注册表过滤接
#define LOG_SPIREGFLT	 	_T("spiregft")
//进程过滤接
#define LOG_SPIPSFLT	 	_T("spipsflt")
//打印过滤钩子
#define LOG_SPPHK		 	_T("spphk")
//MODEM控制
#define LOG_SPMODEM			_T("spmodem")
//资产管理
#define LOG_SPASTMGR	 	_T("spastmgr")
//硬件接口管理
#define LOG_SPHWMGR			_T("sphwmgr")
//安装
#define LOG_SPINSTALL	 	_T("spinstall")
//卸载
#define LOG_SPUNINST	 	_T("spuninst")
//更新
#define LOG_SPUPDATE	 	_T("spupdate")
//本地卸载工具
#define LOG_SPUNIK		 	_T("spunik")
//安全文件柜
#define LOG_SPSECCAB	 	_T("spseccab")
//安全文档
#define LOG_SPVIEWER	 	_T("spviewer")
//病毒检测
#define LOG_SPVIRUS			_T("spvirus")
//补丁管理
#define LOG_SPMEND		 	_T("spmend")
//软件分发
#define LOG_SPSOFTDISP	 	_T("spsoftdisp")
//安全策略
#define LOG_SPSECMON	 	_T("spsecmon")
//消息中心
#define LOG_SPMCENTER	 	_T("spmcenter")
//辅助标识
#define LOG_SPIDTAUX	 	_T("spidtaux")
//授权信息
#define LOG_SPLIC		 	_T("splice")
//失泄密
#define LOG_SPCANTILEAK		_T("spcantileak")
//可信存储
#define LOG_SPCTSTOR	 	_T("spctstor")
//运维管理
#define LOG_SPCRUN		 	_T("spcrun")
//软件分发
#define LOG_SPCSFTDIS	 	_T("spcsftdis")
//补丁管理
#define LOG_SPCMENDMGR	 	_T("spcmendmgr")
//病毒检测
#define LOG_SPCVIRUSMGR		_T("spcvirusmgr")
//防火墙
#define LOG_SPCFIRE			_T("spcfire")
//安全策略
#define LOG_SPCSECPLY	 	_T("spcsecply")
//主服务
#define LOG_SECPATRON	 	_T("secpatron")
//网络通信
#define LOG_SPNETCOMM	 	_T("spnetcomm")
//XML解析
#define LOG_SAXPARSER	 	_T("saxparser")
//用户注册
#define LOG_SPREGUSER	 	_T("spreguser")
//登陆消息通知
#define LOG_SPNOTIFY    	_T("spnotify")
//网络配置控制
#define LOG_SPNETCFG    	_T("spnetcfg")
//远程管理
#define LOG_SPRTMGR    		_T("sprtmgr")
//获取Windows系统日志
#define LOG_SYSEVENTLOG   	_T("syseventlog")
//数据加密
#define LOG_SPDCRYLIB		_T("spdcrylib")
//文件加密
#define LOG_SPFCRYLIB		_T("spfcrylib")
//磁盘总线类型检测
#define LOG_SPDRVTYPE		_T("spdrvtype")
//鼠标键盘、截屏按键
#define LOG_SPKMMGR			_T("spkmmgr")
//安全操作-业务层
#define LOG_SPSECOPT        _T("spsecopt")
//安全操作-核心层
#define LOG_SPCSECOPT       _T("spcsecopt")
//杂项业务处理
#define	LOG_SPCMISC			_T("spcmisc")
//远程管理-业务层
#define LOG_SPCRTMGR		_T("spcrtmgr")
//安全文档
#define LOG_SPCSDOC			_T("spcsdoc")
//安全文档备份
#define LOG_SPSDMGR			_T("spsdmgr")
//保护
#define LOG_SPPROTECT		_T("spprotect")

//可信移动存储
#define  LOG_SPCTSTOR       _T("spctstor")
#define  LOG_SPTSMGR        _T("sptsmgr")
#define  LOG_WBTSMNT        _T("wbtsmnt")

//可信计算
#define  LOG_SPCTCMPT		_T("spctcmpt")

//网络接入认证
#define  LOG_SPNETAUTH		_T("spnetauth")

//打印审批管理核心层
#define LOG_SPPRTAMGR		_T("spprtamgr")

//打印审批管理用户界面
#define LOG_SPPRTAUI		_T("spprtaui")

//光盘刻录管理
#define LOG_SPDISCBURN		_T("spdiscburn")

// 健康性检查业务处理
#define	LOG_SPCPCHL			_T("spcpchl")

// 消息中心可信业务
#define LOG_SPMCTSTOR		_T("spmctstor")

//Windows凭据提供程序
#define LOG_SPCREDPROVIDER	_T("spcredprov")

//审批公共功能
#define LOG_SPAPRCOMM		_T("spapprovecomm")

//飞天key日志
#define	LOG_UKEYFEITIAN		_T("spkeyfeit")

//可信商旅盘日志
#define	LOG_WBTSTRAVEL		_T("WBTSTravel")

//安全文件分发菜单扩展
#define LOG_SPMNFCRY		_T("spmnfcry")

//格尔key管理
#define LOG_SPKOAL			_T("spkoal")

//安全文件传输
#define LOG_SPCFILETRAN		_T("spcfiletran")
#define LOG_SPFTRANMGR		_T("spftranmgr")

#define	WSP_LOG_DIR					_T("WKlog")
#define	WSP_LOG_HTZS				_T("HaoTiZhuShou")
#define	WSP_LOG_OPERATOR				_T("Operator")

#define WRITESPDEBUG(logger) \
 (CSPLogger::getLogger(logger)->writeDebug)
#define WRITESPINFO(logger) \
 (CSPLogger::getLogger(logger)->writeInfo)
#define WRITESPWARN(logger) \
 (CSPLogger::getLogger(logger)->writeWarn)
#define WRITESPERROR(logger) \
 (CSPLogger::getLogger(logger)->writeError)
#define WRITESPFATAL(logger) \
 (CSPLogger::getLogger(logger)->writeFatal)

/**
 * \ingroup Common
 * 日志处理类定义
 *
 * \par requirements
 * win2k or later\n
 *
 * \version 1.0
 * first version
 *
 * \date 2007-05-24
 *
 * \author guanyong
 *
 */
class CSPLogger
{
protected:
	CSPLogger(TCHAR* logname);

	//
	// 错误信息的保护封装类
	//
	class CErrorProtect {
	public:
		CErrorProtect()
		{
			m_dwError = ::GetLastError();
		}

		~CErrorProtect()
		{
			::SetLastError(m_dwError);
		}

	private:
		DWORD m_dwError;
	};

public:
	// 日志级别
	enum SP_LOG_LEVEL {
		LOG_LEVEL_DEBUG,				// DEBUG版默认最小级别
		LOG_LEVEL_INFO,					// Release版默认最小级别
		LOG_LEVEL_WARN,					// 一般性错误
		LOG_LEVEL_ERROR,				// 严重性错误
		LOG_LEVEL_FATAL,				// 致命性错误
		LOG_LEVEL_CLOSED = 128			// 无日志输出
	};

	/**
	 * 获取日志类实例
	 * \param[in] logname 日志文件名称
	 * \return CSPLogger类实例指针，可能为NULL
	 */
	static CSPLogger* getLogger(TCHAR* logname);

	/**
	 * 输出调试信息
	 * \param[in] fromatString 日志格式
	 * \param[in] ... 日志参数
	 */
	void   writeDebug(TCHAR* fromatString,...);

	/**
	* 输出普通信息
	* \param[in] fromatString 日志格式
	* \param[in] ... 日志参数
	*/
	void   writeInfo(TCHAR* fromatString,...);

	/**
	* 输出警告信息
	* \param[in] fromatString 日志格式
	* \param[in] ... 日志参数
	*/
	void   writeWarn(TCHAR* fromatString,...);

	/**
	* 输出错误信息
	* \param[in] fromatString 日志格式
	* \param[in] ... 日志参数
	*/
	void   writeError(TCHAR* fromatString,...);

	/**
	* 输出严重错误信息
	* \param[in] fromatString 日志格式
	* \param[in] ... 日志参数
	*/
	void   writeFatal(TCHAR* fromatString,...);

	/**
	* 设置输出信息级别
	* \param[in] nLogLevel 日志级别
	*/
	void   SetLogLevel(int nLogLevel);

	/**
	* 获取输出信息级别
	*/
	int    GetLogLevel(void) const;

private:
	/**
	 * 输出日志数据
	 * \param[in] szLevel 日志级别
	 * \param[in] szMsg 日志内容
	 */
	void WriteData(const TCHAR* szLevel, const TCHAR* szMsg);

	/**
	 * 检查日志文件状态
	 */
	void CheckLogFile();

	/**
	 * 获取静态日志类实例
	 */
	static inline CSPLogger* GetUnknownLogger();

private:
	int m_nLogLevel;
	int m_nFileSize;
	TCHAR m_szLogFileName[MAX_PATH];
	static std::map<TCHAR*, CSPLogger*> m_loggers;
};

#endif	// _SPLOGGER_H

