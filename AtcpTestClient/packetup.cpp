#include"packetup.h"
#include"rsa.h"
#include<QDebug>
//封包：Client发给AS
void PackUp::PMsgC_AS(char *ID_User,char *LifeTime,char *ID_V,char * ID_Tgs,QByteArray &tmp)
{
    char Packet[11];
    Packet[0]=ID_User[0];
    Packet[1]=ID_User[1];
    for(int i=2;i<6;i++){
        Packet[i]=LifeTime[i-2];
    }
    Packet[6] = ID_V[0];
    Packet[7] = ID_V[1];
    Packet[8] = ID_Tgs[0];
    Packet[9] = ID_Tgs[1];
    Packet[10] = '\0';

    RSA_r rsa;
    //hash
    std::string str = rsa.sha256(LifeTime);
    char * c_s;
    c_s = new char[str.length()+1];
    strcpy(c_s,str.c_str());
    c_s[str.length()] = '\0';

    //rsa加密
    char * c_en;
    c_en = rsa.my_encrypt(c_s,PUBLICKEY);
    qDebug()<<"c_en:  "<<c_en;
    //封包
    char Mess[1024];
    memset(Mess,'\0',sizeof (Mess));
    strcpy(Mess,Packet);
    for(int i = 0;i<1014;i++){
        Mess[i+10] = c_en[i];
    }
    for(int j = 0;j<1024;j++){
        tmp[j] = Mess[j];
    }
}
//拆包c发给as
void PackUp::DMsgC_AS(char *ID_User,char *LifeTime,char *ID_V,char * ID_Tgs,bool& Y,QByteArray tmp){
    char Mess[1024];
    memset(Mess,'\0',sizeof (Mess));
    for(int j = 0;j<1024;j++){
        Mess[j] = tmp[j];
    }
    //id
    ID_User[0] = Mess[0];
    ID_User[1] = Mess[1];
    ID_User[2] = '\0';
    //lifetime
    char life[5];
    for(int i=2;i<6;i++){
        life[i-2]=Mess[i];
    }
    LifeTime[4] = '\0';
    //id_v
    ID_V[0] = Mess[6];
    ID_V[1] = Mess[7];
    ID_V[2] = '\0';
    //id_tgs
    ID_Tgs[0] = Mess[8];
    ID_Tgs[1] = Mess[9];
    ID_Tgs[2] = '\0';


    //hash
    RSA_r rsa;
    std::string str = rsa.sha256(LifeTime);
    QString hs_en = QString::fromStdString(str);

    //提取hash密文
    char c_en[1000];
    for(int i = 10;i<1010;i++){
        c_en[i-10] = Mess[i];
    }

    //decryp
    char * c_de;
    c_de = rsa.my_decrypt(c_en,OPENSSLKEY);
    QString hs_de = QString::fromStdString(c_de);

    //验证客户端
    if(strcmp(hs_de.toLatin1().data(),hs_en.toLatin1().data()) == 0&&strcmp(life,LifeTime)==0){
        Y = true;
    }
    else{
        Y = false;
    }
}


//封包：AS发给Client
void PackUp::PMsgAS_C(char *ID_User,char *ID_TGS,char *Ts,char *LifeTime,char *SessionKey,char *UserKey,char *KDCKey,QByteArray &tmp)
{
    //封装用服务器自己的密钥加密的部分
    char Packet[22];
    Packet[0]=ID_User[0];
    Packet[1]=ID_User[1];
    Packet[2]=ID_TGS[0];
    Packet[3]=ID_TGS[1];
    for(int i=4;i<9;i++){
        Packet[i]=Ts[i-4];
    }
    for(int i=9;i<13;i++){
        Packet[i]=LifeTime[i-9];
    }
    for(int i=13;i<21;i++){
        Packet[i]=SessionKey[i-13];
    }
    Packet[21]='\0';
    //封装用用户密钥加秘密的部分
    char Packet2[20];
    Packet2[0]=ID_TGS[0];
    Packet2[1]=ID_TGS[1];
    for(int i=2;i<7;i++){
        Packet2[i]=Ts[i-2];
    }
    for(int i=7;i<11;i++){
        Packet2[i]=LifeTime[i-7];
    }
    for(int i=11;i<19;i++){
        Packet2[i]=SessionKey[i-11];
    }
    Packet2[19]='\0';
    //服务器密钥加密Packet
    Des->setKey(KDCKey);
    Des->encrypt(Packet);
    const char *Mession=Des->endata.c_str();


    //拼接
    char Mess[1024];
    memset(Mess,'\0',sizeof (Mess));
    strcpy(Mess,Packet2);
    qDebug()<<"packet2:"<<Packet2;
    qDebug()<<"Mession:"<<Mession;
    strcat(Mess,Mession);
    strcat(Mess,"#");


    //hash
    RSA_r rsa;
    std::string str = rsa.sha256(Ts);
    char * c_s;
    c_s = new char[str.length()+1];
    strcpy(c_s,str.c_str());
    c_s[str.length()] = '\0';
    //rsa加密
    char * c_en;
    c_en = rsa.my_encrypt(c_s,PUBLICKEY);


    //拼接
    int i = 0;
    while(Mess[i]!='#'){
        i=i+1;
    }
    i=i+1;
    for(int j =0;j<900;j++){
        Mess[i] = c_en[j];
    }


    //用户密钥加密
    Des->setKey(UserKey);
    Des->encrypt(Mess);
    const char *consequent=Des->endata.c_str();
    //封包
    for(int j = 0;j<1024;j++){
        tmp[j] = consequent[j];
    }
}
//拆包：Client拆AS
void PackUp::DMsgAS_C(char *ID_TGS,char *Ts,char *LifeTime,char *SessionKey,char *UserKey,char *TGT,bool &Y,QByteArray Message)
{
    char consequent[1024];
    memset(consequent,'\0',sizeof (consequent));
    for(int j = 0;j<1023;j++){
        consequent[j] = Message[j];
    }
    //提取密文
    char des_en[1024];
    memset(des_en,'\0',sizeof (des_en));
    int con = 0;
    while(consequent[con] != '#'){
        des_en[con] = consequent[con];
        con++;
    }
    qDebug()<<"des_en :"<<des_en;
    con++;
    int ha = 0;
    char hash_en[1024];
    memset(hash_en,'\0',sizeof (hash_en));
    while(con<900){
        hash_en[ha] = consequent[con];
        ha++;
        con++;
    }
    qDebug()<<"hash_en :"<<hash_en;
    //解密
    Des->setKey(UserKey);
    Des->decrypt(des_en);
    char Pack[20];
    const char *Mess=Des->dedata.c_str();
    qDebug()<<"Mess:"<<Mess;
    for(int i=0;i<19;i++){
        Pack[i]=Mess[i];
    }
    Pack[19]='\0';
    ID_TGS[0] = Pack[0];
    ID_TGS[1] = Pack[1];
    ID_TGS[2] = '\0';
    //提时间戳
    for(int i=2;i<7;i++){
        Ts[i-2]=Pack[i];
    }
    Ts[5]='\0';
    //提lifetime
    for(int i=7;i<11;i++){
        LifeTime[i-7]=Pack[i];
    }
    LifeTime[4]='\0';
    //提sessionkey
    for(int i=11;i<19;i++){
        SessionKey[i-11]=Pack[i];
    }
    SessionKey[8]='\0';
    qDebug()<<"packet:"<<Pack;
    //提取TGT
    memset(TGT,'\0',sizeof(TGT));
    for(int i = 19;i<444;i++){
        TGT[i-19] = Mess[i];
    }
    qDebug()<<"TGT:"<<TGT;

    //hash
    RSA_r rsa;
    std::string str = rsa.sha256(Ts);
    QString hs_en = QString::fromStdString(str);
    qDebug()<<"hs_en"<<hs_en;
    //decryp
    char * c_de;
    c_de = rsa.my_decrypt(hash_en,OPENSSLKEY);
    QString hs_de = QString::fromStdString(c_de);
    qDebug()<<"hs_de"<<hs_de;
    //验证客户端
    if(strcmp(hs_de.toLatin1().data(),hs_en.toLatin1().data()) == 0){
        Y = true;
    }
    else{
        Y = false;
    }
    qDebug()<<"Y"<<Y;
}
//拆包：TGS拆Client
void PackUp::DMsgC_TGS(char *ID_User,char *Ts,char *ID_V,char *Lifetime,char *SessionKey,char *KDCKey,char *TGT_22,QString Message)
{
    //取出密文部分
    int n=0;
    char *tmp=Message.toLatin1().data();
    qDebug()<<"Messa"<<Message.toLatin1().data();
    qDebug()<<"tmp:"<<tmp[1];
    char Mess[1024];
    memset(Mess,'\0',sizeof(Mess));
    for(n=0;n<1024;n++){
        if(tmp[n]!='#'){
            Mess[n]=tmp[n];
        }
        else{
            break;
        }
    }
    qDebug()<<"Mess:"<<Mess;
    //取出明文部分
    char MingWen[7];
    MingWen[0]=tmp[n+1];
    MingWen[1]=tmp[n+2];
    for(int i=2;i<6;i++){
        MingWen[i]=tmp[n+i+1];
    }
    MingWen[6]='\0';
    //取出TGT
    char TGT_tmp[1024];
    memset(TGT_tmp,'\0',sizeof(TGT_tmp));
    qDebug()<<"tmp:"<<tmp;
    for(int i=0;i<1000;i++){
        TGT_tmp[i]=tmp[n+7+i];
    }
    qDebug()<<"TGT_tmp:"<<TGT_tmp;
    //Pack1解密
    Des->setKey(SessionKey);
    qDebug()<<"sessionkey"<<SessionKey;
    Des->decrypt(Mess);
    qDebug()<<"Mess222:"<<Mess;
    char Pack1[8];
    const char *eData=Des->dedata.c_str();
    for(int i=0;i<7;i++){
        Pack1[i]=eData[i];
    }
    Pack1[7]='\0';
    //TGT解密
    Des->setKey(KDCKey);
    Des->decrypt(TGT_tmp);
    const char *Tm=Des->dedata.c_str();
    qDebug()<<Des->dedata.c_str();
    qDebug()<<Tm;
    for(int i=0;i<21;i++){
        TGT_22[i]=Tm[i];
    }
    TGT_22[21]='\0';
    QString aaa;
    aaa=QString(TGT_22);
    //取出有用信息
    ID_User[0]=Pack1[0];
    ID_User[1]=Pack1[1];
    ID_User[2]='\0';
    for(int i=2;i<7;i++){
        Ts[i-2]=Pack1[i];
    }
    Ts[5]='\0';
    ID_V[0]=MingWen[0];
    ID_V[1]=MingWen[1];
    ID_V[2]='\0';
    for(int i=2;i<6;i++){
        Lifetime[i-2]=MingWen[i];
    }
    Lifetime[4]='\0';
}
//封包：Client发给TGS
void PackUp::PMsgC_TGS(char *ID_User,char *Ts,char *ID_V,char *LifeTime,char *SessionKey,char *TGT,QString &tmp)//TGT
{
    //pack1封装
    char Pack1[8];
    Pack1[0]=ID_User[0];
    Pack1[1]=ID_User[1];
    for(int i=2;i<7;i++){
        Pack1[i]=Ts[i-2];
    }
    Pack1[7]='\0';
    //Pack2封装
    char Pack2[7];
    Pack2[0]=ID_V[0];
    Pack2[1]=ID_V[1];
    for(int i=2;i<6;i++){
        Pack2[i]=LifeTime[i-2];
    }
    Pack2[6]='\0';
    //Pack1通过sessionKey加密
    Des->setKey(SessionKey);
    Des->encrypt(Pack1);
    const char *Message=Des->endata.c_str();
    //拼装Pack1密文、Pack2明文、TGT
    char Mess[1024];
    memset(Mess,'\0',sizeof(Mess));
    strcpy(Mess,Message);
    strcat(Mess,"#");
    strcat(Mess,Pack2);
    strcat(Mess,TGT);
    tmp=Mess;
}
//封包:TGS发给Client
void PackUp::PMsgTGS_C(char *ID_User,char *Ts,char *ID_V,char *LifeTime,char *SessionKey,char *ServerKey,QString &tmp)
{
    //封装sesskey加密的Pack1
    char Pack1[20];
    Pack1[0]=ID_V[0];
    Pack1[1]=ID_V[1];
    for(int i=2;i<7;i++){
        Pack1[i]=Ts[i-2];
    }
    for(int i=7;i<11;i++){
        Pack1[i]=LifeTime[i-7];
    }
    for(int i=11;i<19;i++){
        Pack1[i]=ServerKey[i-11];
    }
    Pack1[19]='\0';
    //加密ST
    char ST[22];
    ST[0]=ID_User[0];
    ST[1]=ID_User[1];
    ST[2]=ID_V[0];
    ST[3]=ID_V[1];
    for(int i=4;i<9;i++){
        ST[i]=Ts[i-4];
    }
    for(int i=9;i<13;i++){
        ST[i]=LifeTime[i-9];
    }
    for(int i=13;i<21;i++){
        ST[i]=ServerKey[i-13];
    }
    ST[21]='\0';
    Des->setKey(ServerKey);
    Des->encrypt(ST);
    const char *Mess1=Des->endata.c_str();
    //拼接明文和密文
    char Message[1024];
    memset(Message,'\0',sizeof(Message));
    strcpy(Message,Pack1);
    strcat(Message,Mess1);
    //再次加密
    Des->setKey(SessionKey);
    Des->encrypt(Message);
    const char *Mess2=Des->endata.c_str();
    tmp=Mess2;
}



//拆包：Client拆TGS
void PackUp::DMsgTGS_C(char *ID_V,char *Ts,char *LifeTime,char *SessionKey_CV,char *SessionKey,QString Message)
{
    Des->setKey(SessionKey);
    Des->decrypt(Message.toLatin1().data());
    const char *Mess=Des->dedata.c_str();
    qDebug()<<Mess;
    char Pack1[20];
    for(int i=0;i<19;i++){
        Pack1[i]=Mess[i];
    }
    Pack1[19]='\0';
    char Pack2[1024];
    memset(Pack2,'\0',sizeof(Pack2));
    for(int i=19;i<1000;i++){
        Pack2[i-19]=Mess[i];
    }
    qDebug()<<"Pack1:"<<Pack1;
    qDebug()<<"Pack2:"<<Pack2;
    ID_V[0]=Pack1[0];
    ID_V[1]=Pack1[1];
    ID_V[2]='\0';
    for(int i=2;i<7;i++){
        Ts[i-2]=Pack1[i];
    }
    Ts[5]='\0';
    for(int i=7;i<11;i++){
        LifeTime[i-7]=Pack1[i];
    }
    LifeTime[4]='\0';
    for(int i=11;i<19;i++){
        SessionKey_CV[i-11]=Pack1[i];
    }
    SessionKey_CV[8]='\0';
}
