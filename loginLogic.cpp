#include "loginLogic.h"

loginLogic::loginLogic()
{
	m_sql = sqliteManage::getInstance();
}

loginLogic::~loginLogic()
{

}

void loginLogic::readPwd(QString& account, QString& pwd)
{
    QSettings cfg("./config/config.ini", QSettings::IniFormat);

    account =  cfg.value("account").toString();
    pwd = cfg.value("pwd").toString();
}

bool loginLogic::login(bool save,const QString& account, const QString& pwd)
{
    auto pwdMD5 = MD5_encryption(pwd);
    QString s = QString(u8"SELECT id FROM user WHERE account = '%1'AND password = '%2';").arg(account).arg(pwdMD5);

    QSqlQuery query;
    int userID = 0;
    if (query.exec(s)) {
        while (query.next())
        {
            userID = query.value(0).toInt();
        }
    }
    else {
        //打印sql语句错误信息
        qDebug() << "init table error" << query.lastError();
    }
    if (userID != 0)
    {
        if (save)
        {
            savePwd(account, pwd);
        }
        return true;
    }
    return false;
}

void loginLogic::savePwd(const QString& account, const QString& pwd)
{
    QSettings cfg( "./config/config.ini", QSettings::IniFormat);
    cfg.setValue("account", account);
    cfg.setValue("pwd", pwd);
	return;
}

QString loginLogic::MD5_encryption(const QString& data)
{
    QCryptographicHash md5(QCryptographicHash::Md5);    //使用MD5加密
    md5.addData(data.toUtf8(), data.size());             // 添加数据
    QByteArray bArry = md5.result();                    //获取MD5加密后的密码
    QString md5_data;
    md5_data.append(bArry.toHex());
    return md5_data;
}
