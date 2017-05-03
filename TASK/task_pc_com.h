#ifndef _TASK_PC_COM_H
#define _TASK_PC_COM_H

#include "includes.h"
#include "mj_lib.h"

/**********************PC��С��֮���ͨѶ�ṹ��*************************/
#define PC_CMD_DATA_LEN          256          //���ݰ���󳤶�

#define PC_CMD_HEAD              0xaa					// ��ͷ��ʶ

/*���ݰ�״̬�ظ�*/
#define PC_STATUS_SUCCESS       0x00					// ���ݰ�����ok
#define PC_STATUS_ERROR         0x80					// ���ݰ��������
#define PC_STATUS_ERR_CMD       0x81					// ��֧�ִ�����
#define PC_STATUS_ERR_BCC       0x82					// ������У�����
#define PC_STATUS_ERR_OPT     	0x83					// �������ʧ��

/*���ݵ�˳��head,seq,cmd,state,len,data,bcc*/
typedef struct _pc_cmd_t 
{
	unsigned char  head;				                // ��ͷ��ʶ,�̶�ֵ0xaa
	unsigned char  seq;					                // �����к�
	unsigned char  cmd;					                // ������,������
	unsigned char	 state;		  	                // ��״̬,�����ظ���λ��״̬��			
	unsigned char  len;					                // �������򳤶�
	unsigned char  data[PC_CMD_DATA_LEN];	      // ��������
	unsigned char  bcc;                         // У��λ,add by wqj,2017-03-27
}pc_cmd_t;

typedef enum 
{
	PC_CMD_CODE_SYNC = 0, 								      // ����
	// RMS->PC                                  
	PC_CMD_CORD_SENT_RMS_STATE = 1,				      // ����λ������RMS״̬
	PC_CMD_CORD_SENT_TALKBACK_REQ,				      // ����λ�����������Խ�����
	PC_CMD_CORD_SENT_ALAM_MSG,				          // ����λ�����ͱ�����Ϣ
	
	// PC->RMS
	PC_CMD_CORD_SET_RMS_NAVIGATION = 0x80,      //��̨�������ƣ�add by gao
	PC_CMD_CORD_SET_RMS_CONTROL	= 0x81,				  // ��λ������RMS , PC->RMS
	PC_CMD_CORD_SET_EMG_BRACK	,				          // RMS�����ƶ�, PC -> RMS
	PC_CMD_CORD_SET_F_WHEEL_TURN,				        // ��λ������С��ǰ��ת��
	PC_CMD_CORD_SET_B_WHEEL_SPEED,				      // ��λ������С�������ٶ�
	PC_CMD_CORD_SET_FRONT_LIGHT = 0x85,				  // ��λ������ǰ���
	PC_CMD_CORD_SET_BACK_LIGHT,				          // ����
	PC_CMD_CORD_SET_BRAKE_LIGHT,				        // ɲ����
	PC_CMD_CORD_SET_TURN_LEFT_LIGHT,				    // ��ת��
	PC_CMD_CORD_SET_TURN_RIGHT_LIGHT,				    // ��ת��
	PC_CMD_CORD_SET_SOUND_LIGHT_ALARM=0x8A,	    // ���ⱨ��        
	PC_CMD_CORD_SET_UPL_STATE=0x8B,						  // ���ó�����״̬      
	PC_CMD_CORD_SET_TALKBACK_MODULE,				    // �����Խ�    
                                              
	PC_CMD_CORD_SET_SYS_MODE,				            // 0x8D�趨��ǰģʽ
	PC_CMD_CORD_SET_YUNTAI,				              // 0x8E ��̨
	PC_CMD_CORD_SET_DIRECTION_SPEED,				    // 0x8F	��λ������С��ת��������ٶ�
	PC_CMD_CORD_SET_HIGH_DECIBEL_SPEAKER=0x90,  // �߷ֱ�����     
	PC_CMD_CORD_SET_BRIGHT_LIGHT ,              // 0X91 ��λ������ǿ��ƺͺ��ⲹ���
	PC_CMD_CORD_SET_INFRARED_LIGHT,             // 0X92 ��λ������ǿ��ƺͺ��ⲹ���
	PC_CMD_CORD_SET_SPEAKER_MUTE,               // 0X93 ��λ�����Ƹ����㲥����
	PC_CMD_CORD_SET_NAVI_BRIGHT_LIGHT,          // 0X94 ��λ�����Ƶ���ǿ���

	PC_CMD_CODE_MAX_NO,                         // ���ֵ��ʼ�շ������   

}pc_cmd_type;

typedef  handler_result_t (*pc_cmd_handler_event)(pc_cmd_t *in_out_pcmd);
typedef struct _pc_cmd_handler_t 
{
	pc_cmd_type              	cmd;
	pc_cmd_handler_event        event;
}pc_cmd_handler_t;

#define PC_CMD_INVALID(c)          ((c >= PC_CMD_CORD_SENT_RMS_STATE) && (c < PC_CMD_CODE_MAX_NO))
#define IR313_CMD_INVALID(c)       ((c >= 0x27) && (c < 0x30))  






// �����Խ�, RMS->pc,  0x02
typedef __packed  struct _talkback {
	u8 	robot_no:4;			                         // �����˱��
	u8	track_no:4;			                         // ������
	u32 unixtime;			                           // uinix timer
	float latitude;			                         // γ��, ��γΪ��,��γΪ��
	float longitude;		                         // ����, ����Ϊ��
	u8   retain;			                           // Ԥ��
	u8	talkback_state;		                       // �Խ�����
}talkback_t, *ptalkback_t;

typedef __packed  struct _rms_ctl {
	u8 	robot_no:4;			                         // �����˱��
	u8	track_no:4;			                         // ������
	u32 unixtime;			                           // uinix timer
	s8	angle;				                           // ǰ�ֽǶ�
	s16	speed;				                           // �����ٶ� L/s
                                               
	u8  high_dec_speaker:1;	                     // �������� 0:�رգ�1���� , byte ���λ,bit0
	u8	light_alarm:1;		                       // �ⱨ��	0:�رգ�1����	, bit1
	u8	right_light:1;		                       // ��ת���, byte�����λ	, bit2
	u8	left_light:1;		                         // ��ת���
	u8	brake_light:1;		                       // ɲ����
	u8	back_light:1;		                         // ����
	u8	front_light:2;		                       // ǰ��		0:�رգ� 1:��	, byte���λ,bit7
                                               
	u8  retain:5;			                           // Ԥ��
	u8	yuntai_pos:1;		                         // ��̨����, 0:������͵�, 1������ߵ�
	u8	rms_mode:1;			                         // ģʽ, 0:�Զ�ģʽ��1:�ֶ�ģʽ, bit6
	u8  talk_back:1;		                         // �����Խ� 0:�رգ�1���� ,  byte���λ,bit7	
}rms_ctl_t, *prms_ctl_t;

typedef __packed struct _sensor_ctl 
{
	u8  high_dec_speaker:1;	// ???? 0:??,1:?? , byte ???,bit0
	u8	light_alarm:1;		// ???	0:??,1:??	, bit1
	u8	right_light:1;		// ????, byte????	, bit2
	u8	left_light:1;		// ????
	u8	brake_light:1;		// ???
	u8	back_light:1;		// ???
	u8	front_light:2;		// ??		00:??, 01:??, 10: ??	, byte???,bit7,bit6

	u8  retain:7;			// ??
	u8  talk_back:1;		// ???? 0:??,1:?? ,  byte???,bit7	
}sensor_ctl_t, *psensor_ctl_t;

typedef struct _driver_ctl {
	s8	angle;	
	s16	speed;
	u8  retain:7;
	u8	rms_mode:1;
}driver_ctl_t, *pdriver_ctl_t;
// �����˽����ƶ����� PC->RMS�� ������0x82
typedef __packed struct _rms_emg_brake {
	u8	ctl_mod;			                           // ����ģʽ, 0x00 ֹͣ���л�����,0x01����������
	u8 	robot_no;			                           // �����˱��
	u32 unixtime;			                           // uinix timer
}rms_emg_brake_t, *prms_emg_brake_t;

// ϵͳģʽ
typedef enum 
{
	AUTO_RUN,		                                // �Զ�ѭ��ģʽ
	HUMAN_CTL,		                              // ��̨����ģʽ(��̨�˹�����С���˶�)
}sys_run_mode_t	;

void pc_cmd_send_char(unsigned char ch);
void pc_init(void);
unsigned char pc_cmd_handler(void);


// RMS->PC, ���̨�����������Խ�����
void pc_sent_talkback_request(talkback_t tb);
void pc_sent_rms_state(rms_state_t rms_state);
void pc_sent_alarm_msg(alarm_t alarm, u8 *data);

//RMS->IR313, ��ҹ���Ƿ�������
void ir313_sent_8_msg(u8 cmdtype);
void ir313_sent_9_msg(u8 cmdtype ,u8 cmddata);
void ir313_sent_10_msg(u8 cmdtype ,u8 cmddata1 ,u8 cmddata2);
void ir313_sent_11_msg(u8 cmdtype ,u8 cmddata1 ,u8 cmddata2 ,u8 cmddata3);

u8 ir313_cmd_decoder(void);
u8 fpl330_cmd_decoder(void);
u8 gps_cmd_decoder(void);
// RMS��ir313ͨ�����ݰ��ṹ��(����Ŀ�����ͱ���������)
typedef struct _ir313_cmd_t {
	unsigned char               head[2];				// ��Ϣͷ
	unsigned char               cmd;					  // ������			
	unsigned char               len[2];					// �������򳤶�
	unsigned char               data[256];	    // ��������
	unsigned char               bcc;					  // У���
	unsigned char               tail[2];				// ������־
}ir313_cmd_t, *pir313_cmd_t;

// RMS��fpl330ͨ�����ݰ��ṹ��
typedef struct _fpl330_cmd_t {
	unsigned char               head;					  // ���̴���
	unsigned char								state;          // ״̬λ
	unsigned char               reserve;			  // ����			
	unsigned char               fun;					  // ����
	unsigned char               len[2];				  // λ����
	unsigned char               crc1[2];			  // У���	
	unsigned char               data[2];		    // ��������
	unsigned char               crc2[2];			  // У���
}fpl330_cmd_t, *pfpl330_cmd_t;

// RMS��GPS	������ͨ�����ݰ��ṹ��(����GPS��ʱ��;�γ�ȵ���Ϣ)
typedef struct _gps_cmd_t {
	unsigned char               head;					   // ��ͷ��ʶ	
	unsigned char               cmd;					   // ������,������	
	unsigned char               len;					   // �������򳤶�
	unsigned char               data[60];	       // ��������
	unsigned char         			crc[2];          //crcУ���� 
}gps_cmd_t, *pgps_cmd_t;

// RMS��ir313ͨ�����ݰ��ṹ�壨8�ֽڳ���ʱ�ã�
typedef struct _ir313_cmd_t_8 {
	unsigned char               head[2];				 // ��Ϣͷ
	unsigned char               cmd;					   // ������			
	unsigned char               len[2];					 // �������򳤶�
	unsigned char               bcc;					   // У���
	unsigned char               tail[2];				 // ������־
}ir313_cmd_t_8, *pir313_cmd_t_8;

// RMS��ir313ͨ�����ݰ��ṹ�壨9�ֽڳ���ʱ�ã�
typedef struct _ir313_cmd_t_9 {
	unsigned char               head[2];				 // ��Ϣͷ
	unsigned char               cmd;					   // ������			
	unsigned char               len[2];					 // �������򳤶�
	unsigned char 							data;            
	unsigned char               bcc;					   // У���
	unsigned char               tail[2];				 // ������־
}ir313_cmd_t_9, *pir313_cmd_t_9;               
                                               
// RMS��ir313ͨ�����ݰ��ṹ�壨10�ֽڳ���ʱ�ã�
typedef struct _ir313_cmd_t_10 {               
	unsigned char               head[2];				 // ��Ϣͷ
	unsigned char               cmd;					   // ������			
	unsigned char               len[2];					 // �������򳤶�
	unsigned char 							data[2];         
	unsigned char               bcc;					   // У���
	unsigned char               tail[2];				 // ������־
}ir313_cmd_t_10, *pir313_cmd_t_10;             
                                               
// RMS��ir313ͨ�����ݰ��ṹ�壨10�ֽڳ���ʱ�ã�
typedef struct _ir313_cmd_t_11 {               
	unsigned char               head[2];				 // ��Ϣͷ
	unsigned char               cmd;					   // ������			
	unsigned char               len[2];					 // �������򳤶�
	unsigned char 							data[3];         
	unsigned char               bcc;					   // У���
	unsigned char               tail[2];				 // ������־
}ir313_cmd_t_11, *pir313_cmd_t_11;


/*pc com rx task �������,�����PC��������ָ��*/
#define PC_COMRx_STK_SIZE 		256

extern OS_TCB  PcComRxTaskTCB;                
extern CPU_STK PC_COMRx_TASK_STK[PC_COMRx_STK_SIZE];

void pc_comRx_task(void *p_arg);               

/*pc com tx task ������ã���ʱ��PC����ȫ״̬��Ϣ*/
#define PC_COMTx_STK_SIZE 		512

extern rms_state_t rms_state; 				         // ȫ״̬

extern OS_TCB  PcComTxTaskTCB;                        
extern CPU_STK PC_COMTx_TASK_STK[PC_COMTx_STK_SIZE];


void pc_comRx_task(void *p_arg);
void pc_comTx_task(void *p_arg);













handler_result_t pc_cmd_handler_sync(pc_cmd_t *in_out_pcmd);            	     
handler_result_t pc_cmd_handler_sent_rms_state(pc_cmd_t *in_out_pcmd);          
handler_result_t pc_cmd_handler_sent_talkback_req(pc_cmd_t *in_out_pcmd); 
handler_result_t pc_cmd_handler_set_navigation(pc_cmd_t *in_out_pcmd);         
handler_result_t pc_cmd_handler_set_rms_control(pc_cmd_t *in_out_pcmd); 		     
handler_result_t pc_cmd_handler_set_emergency_brake(pc_cmd_t *in_out_pcmd);      
handler_result_t pc_cmd_handler_set_f_wheel_turn(pc_cmd_t *in_out_pcmd); 	       
handler_result_t pc_cmd_handler_set_b_wheel_speed(pc_cmd_t *in_out_pcmd); 	     
handler_result_t pc_cmd_handler_set_front_light(pc_cmd_t *in_out_pcmd); 	       
handler_result_t pc_cmd_handler_set_back_light(pc_cmd_t *in_out_pcmd); 		       
handler_result_t pc_cmd_handler_set_brake_light(pc_cmd_t *in_out_pcmd); 	       
handler_result_t pc_cmd_handler_set_turn_left_light(pc_cmd_t *in_out_pcmd);      
handler_result_t pc_cmd_handler_set_turn_right_light(pc_cmd_t *in_out_pcmd);     
handler_result_t pc_cmd_handler_set_sound_light_alarm(pc_cmd_t *in_out_pcmd);    
handler_result_t pc_cmd_handler_set_upl_state(pc_cmd_t *in_out_pcmd);            
handler_result_t pc_cmd_handler_set_talkback_module(pc_cmd_t *in_out_pcmd);      
handler_result_t pc_cmd_handler_set_sys_mode(pc_cmd_t *in_out_pcmd); 		         
handler_result_t pc_cmd_handler_set_yuntai(pc_cmd_t *in_out_pcmd); 			         
handler_result_t pc_cmd_handler_set_direction_speed(pc_cmd_t *in_out_pcmd);      
handler_result_t pc_cmd_handler_set_high_decibel_speaker(pc_cmd_t *in_out_pcmd); 
handler_result_t pc_cmd_handler_set_bright_light(pc_cmd_t *in_out_pcmd);         
handler_result_t pc_cmd_handler_set_infrared_light(pc_cmd_t *in_out_pcmd);      
handler_result_t pc_cmd_handler_set_speaker_mute(pc_cmd_t *in_out_pcmd);         
handler_result_t pc_cmd_handler_set_navi_bright_light(pc_cmd_t *in_out_pcmd); 
#endif
