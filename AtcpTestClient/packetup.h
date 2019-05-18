 #ifndef PACKETUP_H
#define PACKETUP_H
#include<QString>
#include"descrypt.h"
class PackUp{
public:
    PackUp(){}
    //封包
    void PMsgC_AS(char *ID_User,char *LifeTime,char *ID_V,char * ID_Tgs,QByteArray &tmp); //输入ID_User,LifeTime封包得到tmp;
    void PMsgAS_C(char *ID_User,char *ID_TGS,char *Ts,char *LifeTime,char *SessionKey,char *UserKey,char *KDCKey,QByteArray &tmp);
    void PMsgC_TGS(char *ID_User,char *Ts,char *ID_V,char *LifeTime,char *SessionKey,char *TGT,QString &tmp);//TGT
    void PMsgTGS_C(char *ID_User,char *Ts,char *ID_V,char *LifeTime,char *SessionKey,char *ServerKey,QString &tmp);

    //拆包
    void DMsgC_AS(char *ID_User,char *LifeTime,char *ID_V,char * ID_Tgs,bool& Y,QByteArray tmp);
    void DMsgAS_C(char *ID_TGS,char *Ts,char *LifeTime,char *SessionKey,char *UserKey,char *TGT,bool& Y,QByteArray Message);
    void DMsgC_TGS(char *ID_User,char *Ts,char *ID_V,char *Lifetime,char *SessionKey,char *KDCKey,char *TGT_22,QString Message);
    void DMsgTGS_C(char *ID_V,char *Ts,char *LifeTime,char *SessionKey_CV,char *SessionKey,QString Message);

private:

    DesCrypt *Des=new DesCrypt();

};

#endif // PACKETUP_H
