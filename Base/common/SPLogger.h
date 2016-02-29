/** 
 * \file SPLogger.h
 * \brief ������־��ͷ�ļ�����
 *
 * ����ϵͳ����־���
 */

//#pragma once
#ifndef _SPLOGGER_H
#define _SPLOGGER_H

#include <stdlib.h>
#include <tchar.h>
#include <map>
#include <wtypes.h>


///////////////////////////////////////////////////////
//��־����������
///////////////////////////////////////////////////////

//�������ݷ���ģ����־����
#define LOG_SPDAMGR         _T("spdamgr")
//�ļ����˹���
#define LOG_SPFHK		 	_T("spfhk")
//�ļ����˹���
#define LOG_SPFLT		 	_T("spfflt")
//������ƽ�
#define LOG_SPNETFLT	 	_T("spnetflt")
//ע�����˽�
#define LOG_SPIREGFLT	 	_T("spiregft")
//���̹��˽�
#define LOG_SPIPSFLT	 	_T("spipsflt")
//��ӡ���˹���
#define LOG_SPPHK		 	_T("spphk")
//MODEM����
#define LOG_SPMODEM			_T("spmodem")
//�ʲ�����
#define LOG_SPASTMGR	 	_T("spastmgr")
//Ӳ���ӿڹ���
#define LOG_SPHWMGR			_T("sphwmgr")
//��װ
#define LOG_SPINSTALL	 	_T("spinstall")
//ж��
#define LOG_SPUNINST	 	_T("spuninst")
//����
#define LOG_SPUPDATE	 	_T("spupdate")
//����ж�ع���
#define LOG_SPUNIK		 	_T("spunik")
//��ȫ�ļ���
#define LOG_SPSECCAB	 	_T("spseccab")
//��ȫ�ĵ�
#define LOG_SPVIEWER	 	_T("spviewer")
//�������
#define LOG_SPVIRUS			_T("spvirus")
//��������
#define LOG_SPMEND		 	_T("spmend")
//����ַ�
#define LOG_SPSOFTDISP	 	_T("spsoftdisp")
//��ȫ����
#define LOG_SPSECMON	 	_T("spsecmon")
//��Ϣ����
#define LOG_SPMCENTER	 	_T("spmcenter")
//������ʶ
#define LOG_SPIDTAUX	 	_T("spidtaux")
//��Ȩ��Ϣ
#define LOG_SPLIC		 	_T("splice")
//ʧй��
#define LOG_SPCANTILEAK		_T("spcantileak")
//���Ŵ洢
#define LOG_SPCTSTOR	 	_T("spctstor")
//��ά����
#define LOG_SPCRUN		 	_T("spcrun")
//����ַ�
#define LOG_SPCSFTDIS	 	_T("spcsftdis")
//��������
#define LOG_SPCMENDMGR	 	_T("spcmendmgr")
//�������
#define LOG_SPCVIRUSMGR		_T("spcvirusmgr")
//����ǽ
#define LOG_SPCFIRE			_T("spcfire")
//��ȫ����
#define LOG_SPCSECPLY	 	_T("spcsecply")
//������
#define LOG_SECPATRON	 	_T("secpatron")
//����ͨ��
#define LOG_SPNETCOMM	 	_T("spnetcomm")
//XML����
#define LOG_SAXPARSER	 	_T("saxparser")
//�û�ע��
#define LOG_SPREGUSER	 	_T("spreguser")
//��½��Ϣ֪ͨ
#define LOG_SPNOTIFY    	_T("spnotify")
//�������ÿ���
#define LOG_SPNETCFG    	_T("spnetcfg")
//Զ�̹���
#define LOG_SPRTMGR    		_T("sprtmgr")
//��ȡWindowsϵͳ��־
#define LOG_SYSEVENTLOG   	_T("syseventlog")
//���ݼ���
#define LOG_SPDCRYLIB		_T("spdcrylib")
//�ļ�����
#define LOG_SPFCRYLIB		_T("spfcrylib")
//�����������ͼ��
#define LOG_SPDRVTYPE		_T("spdrvtype")
//�����̡���������
#define LOG_SPKMMGR			_T("spkmmgr")
//��ȫ����-ҵ���
#define LOG_SPSECOPT        _T("spsecopt")
//��ȫ����-���Ĳ�
#define LOG_SPCSECOPT       _T("spcsecopt")
//����ҵ����
#define	LOG_SPCMISC			_T("spcmisc")
//Զ�̹���-ҵ���
#define LOG_SPCRTMGR		_T("spcrtmgr")
//��ȫ�ĵ�
#define LOG_SPCSDOC			_T("spcsdoc")
//��ȫ�ĵ�����
#define LOG_SPSDMGR			_T("spsdmgr")
//����
#define LOG_SPPROTECT		_T("spprotect")

//�����ƶ��洢
#define  LOG_SPCTSTOR       _T("spctstor")
#define  LOG_SPTSMGR        _T("sptsmgr")
#define  LOG_WBTSMNT        _T("wbtsmnt")

//���ż���
#define  LOG_SPCTCMPT		_T("spctcmpt")

//���������֤
#define  LOG_SPNETAUTH		_T("spnetauth")

//��ӡ����������Ĳ�
#define LOG_SPPRTAMGR		_T("spprtamgr")

//��ӡ���������û�����
#define LOG_SPPRTAUI		_T("spprtaui")

//���̿�¼����
#define LOG_SPDISCBURN		_T("spdiscburn")

// �����Լ��ҵ����
#define	LOG_SPCPCHL			_T("spcpchl")

// ��Ϣ���Ŀ���ҵ��
#define LOG_SPMCTSTOR		_T("spmctstor")

//Windowsƾ���ṩ����
#define LOG_SPCREDPROVIDER	_T("spcredprov")

//������������
#define LOG_SPAPRCOMM		_T("spapprovecomm")

//����key��־
#define	LOG_UKEYFEITIAN		_T("spkeyfeit")

//������������־
#define	LOG_WBTSTRAVEL		_T("WBTSTravel")

//��ȫ�ļ��ַ��˵���չ
#define LOG_SPMNFCRY		_T("spmnfcry")

//���key����
#define LOG_SPKOAL			_T("spkoal")

//��ȫ�ļ�����
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
 * ��־�����ඨ��
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
	// ������Ϣ�ı�����װ��
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
	// ��־����
	enum SP_LOG_LEVEL {
		LOG_LEVEL_DEBUG,				// DEBUG��Ĭ����С����
		LOG_LEVEL_INFO,					// Release��Ĭ����С����
		LOG_LEVEL_WARN,					// һ���Դ���
		LOG_LEVEL_ERROR,				// �����Դ���
		LOG_LEVEL_FATAL,				// �����Դ���
		LOG_LEVEL_CLOSED = 128			// ����־���
	};

	/**
	 * ��ȡ��־��ʵ��
	 * \param[in] logname ��־�ļ�����
	 * \return CSPLogger��ʵ��ָ�룬����ΪNULL
	 */
	static CSPLogger* getLogger(TCHAR* logname);

	/**
	 * ���������Ϣ
	 * \param[in] fromatString ��־��ʽ
	 * \param[in] ... ��־����
	 */
	void   writeDebug(TCHAR* fromatString,...);

	/**
	* �����ͨ��Ϣ
	* \param[in] fromatString ��־��ʽ
	* \param[in] ... ��־����
	*/
	void   writeInfo(TCHAR* fromatString,...);

	/**
	* ���������Ϣ
	* \param[in] fromatString ��־��ʽ
	* \param[in] ... ��־����
	*/
	void   writeWarn(TCHAR* fromatString,...);

	/**
	* ���������Ϣ
	* \param[in] fromatString ��־��ʽ
	* \param[in] ... ��־����
	*/
	void   writeError(TCHAR* fromatString,...);

	/**
	* ������ش�����Ϣ
	* \param[in] fromatString ��־��ʽ
	* \param[in] ... ��־����
	*/
	void   writeFatal(TCHAR* fromatString,...);

	/**
	* ���������Ϣ����
	* \param[in] nLogLevel ��־����
	*/
	void   SetLogLevel(int nLogLevel);

	/**
	* ��ȡ�����Ϣ����
	*/
	int    GetLogLevel(void) const;

private:
	/**
	 * �����־����
	 * \param[in] szLevel ��־����
	 * \param[in] szMsg ��־����
	 */
	void WriteData(const TCHAR* szLevel, const TCHAR* szMsg);

	/**
	 * �����־�ļ�״̬
	 */
	void CheckLogFile();

	/**
	 * ��ȡ��̬��־��ʵ��
	 */
	static inline CSPLogger* GetUnknownLogger();

private:
	int m_nLogLevel;
	int m_nFileSize;
	TCHAR m_szLogFileName[MAX_PATH];
	static std::map<TCHAR*, CSPLogger*> m_loggers;
};

#endif	// _SPLOGGER_H

