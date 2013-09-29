#pragma once

#ifdef UNICODE
#define CP(TEXT) _T(TEXT)
#else
#define CP(TEXT) TEXT
#endif // DEBUG

#define DDEID_RISEFALL CP(".184") //���^
#define DDEID_STRIKEPC CP(".125") //�����
#define DDEID_VOLUME CP(".404") //����q
#define DDEID_BUYPC CP(".101") //�R��
#define DDEID_SELLPC CP(".102") //���
#define DDEID_TIME CP(".143") //�ɶ�
#define DDEID_CODE CP(".1018") //�N�X
#define DDEID_NAME CP(".2020") //�W��

#define DDENAME_RISEFALL CP("���^") //.184
#define DDENAME_STRIKEPC CP("�����") //.125
#define DDENAME_VOLUME CP("����q") //.404
#define DDENAME_BUYPC CP("�R��") //.101
#define DDENAME_SELLPC CP("���") //.102
#define DDENAME_TIME CP("�ɶ�") //.143
#define DDENAME_CODE CP("�N�X") //.1018
#define DDENAME_NAME CP("�W��") //.2020

#define JY_AMP_ID CP("M1JY&")
#define JY_AMP(DDEID) CP("M1JY&")DDEID

#define JY_AMP_RISEFALL JY_AMP(DDEID_RISEFALL) //���^
#define JY_AMP_STRIKEPC JY_AMP(DDEID_STRIKEPC) //�����
#define JY_AMP_VOLUME JY_AMP(DDEID_VOLUME) //����q
#define JY_AMP_BUYPC JY_AMP(DDEID_BUYPC) //�R��
#define JY_AMP_SELLPC JY_AMP(DDEID_SELLPC) //���
#define JY_AMP_TIME JY_AMP(DDEID_TIME) //�ɶ�
#define JY_AMP_CODE JY_AMP(DDEID_CODE) //�N�X
#define JY_AMP_NAME JY_AMP(DDEID_NAME) //�W��

#define GET_NAME_FROM_ID(id) \
((id.Right(4)==DDEID_RISEFALL)? DDENAME_RISEFALL :\
(id.Right(4)==DDEID_STRIKEPC)? DDENAME_STRIKEPC :\
(id.Right(4)==DDEID_VOLUME)? DDENAME_VOLUME :\
(id.Right(4)==DDEID_BUYPC)? DDENAME_BUYPC :\
(id.Right(4)==DDEID_SELLPC)? DDENAME_SELLPC :\
(id.Right(4)==DDEID_TIME)? DDENAME_TIME :\
(id.Right(5)==DDEID_CODE)? DDENAME_CODE :\
(id.Right(5)==DDEID_NAME)? DDENAME_NAME :\
CP(""))

#define GET_ID_FROM_NAME(name) \
((name==DDENAME_RISEFALL)? DDEID_RISEFALL:\
(name==DDENAME_STRIKEPC)? DDEID_STRIKEPC:\
(name==DDENAME_VOLUME)? DDEID_VOLUME:\
(name==DDENAME_BUYPC)? DDEID_BUYPC:\
(name==DDENAME_SELLPC)? DDEID_SELLPC:\
(name==DDENAME_TIME)? DDEID_TIME:\
(name==DDENAME_CODE)? DDEID_CODE:\
(name==DDENAME_NAME)? DDEID_NAME:\
CP(""))