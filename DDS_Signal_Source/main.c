#include "include.h"


// I2S ʹ��2�� slot
#define I2S_SLOTS                             (2u)

// ���� ÿ�� slot ��С
#define SLOT_SIZE                             (16u)

// �������� word ��С. Word size <= Slot size
#define WORD_SIZE                             (16u)

//ÿ����������ֽ���
#define BYTES_PER_SAMPLE                      ((SLOT_SIZE >> 3))

// AIC3106 ��ƵоƬ�� I2C �ӵ�ַ
#define I2C_SLAVE_CODEC_AIC31                 (0x18u)

#define WAVE_START_ADDR   ((unsigned int)outwave)
#define WAVE_NUM_BYTES    (sizeof(outwave))
#define WAVE_END_ADDR     (WAVE_START_ADDR + WAVE_NUM_BYTES - 1)
#define PaRAM_BCNT        (2u)
#define PaRAM_CCNT        (1u)

extern double sintable[4096]={0};
extern unsigned short  outwave[2]={0};
extern double squaretable[4096]={0};

static struct EDMA3CCPaRAMEntry EDMA3PaRAM =
{
		(unsigned int)(EDMA3CC_OPT_DAM  | (0x02 << 8u)),	// Opt
		(unsigned int)WAVE_START_ADDR,						// Դ��ַ
		(unsigned short)BYTES_PER_SAMPLE,				    // aCnt
		(unsigned short)PaRAM_BCNT,						// bCnt
		(unsigned int) SOC_MCASP_0_DATA_REGS,				// Ŀ���ַ
	    (short) BYTES_PER_SAMPLE,						    // Դ bIdx
		(short)0x00,											// Ŀ�� bIdx
		(unsigned short)(32u * 1u),						// ���ӵ�ַ
		(unsigned short)PaRAM_BCNT,						// bCnt ��װֵ
		(short)BYTES_PER_SAMPLE,						        // Դ cIdx
		(short)0x00,											// Ŀ�� cIdx
		(unsigned short)PaRAM_CCNT							// cCnt
};




static void InitAIC31I2S(void);
static void InitMCASP(void);
static void InterruptInit(void);
void InitSinTable();
static void Delay(volatile unsigned int delay);
/*
 * main.c
 */
int main(void)
{
	UARTStdioInit();
	UARTPuts("\r\n ============Test Start===========.\r\n", -1);
	InterruptInit();
	InitSinTable();
	I2CPinMuxSetup(0);
	I2CSetup(SOC_I2C_0_REGS, I2C_SLAVE_CODEC_AIC31);
	I2CIntRegister(C674X_MASK_INT7, SYS_INT_I2C0_INT);
	InitAIC31I2S();
	InitMCASP();
	Timer64Init();
	TimerInterruptInit();
	Delay(0xffff);
	while(1)
	{

	}
}

static void InterruptInit(void)
{
	// ��ʼ�� DSP �жϿ�����
	IntDSPINTCInit();

	// ʹ�� DSP ȫ���ж�
	IntGlobalEnable();
}


static void InitAIC31I2S(void)
{
    volatile unsigned int delay = 0xFFF;

    // ��λ
    AIC31Reset(SOC_I2C_0_REGS);
    while(delay--);

    // ��ʼ�� AIC31 Ϊ I2S ģʽ
    AIC31DataConfig(SOC_I2C_0_REGS, AIC31_DATATYPE_I2S, SLOT_SIZE, 0);

    // ��ʼ��������Ϊ 48000Hz
    AIC31SampleRateConfig(SOC_I2C_0_REGS, AIC31_MODE_BOTH, FS_44100_HZ);

    // ��ʼ�� ADC 0�ֱ�����,���� LINE IN
	AIC31ADCInit(SOC_I2C_0_REGS, ADC_GAIN_0DB, AIC31_LINE_IN);

	// ��ʼ�� DAC 0�ֱ�˥��
	AIC31DACInit(SOC_I2C_0_REGS, DAC_ATTEN_0DB);
}

static void InitMCASP()
{
	McASPPinMuxSetup();
	PSCModuleControl(SOC_PSC_0_REGS, HW_PSC_CC0, PSC_POWERDOMAIN_ALWAYS_ON,
				 PSC_MDCTL_NEXT_ENABLE);
	PSCModuleControl(SOC_PSC_0_REGS, HW_PSC_TC0, PSC_POWERDOMAIN_ALWAYS_ON,
				 PSC_MDCTL_NEXT_ENABLE);
	EDMA3RequestChannel(SOC_EDMA30CC_0_REGS, EDMA3_CHANNEL_TYPE_DMA,
					EDMA3_CHA_MCASP0_TX, EDMA3_CHA_MCASP0_TX, 0);
	EDMA3SetPaRAM(SOC_EDMA30CC_0_REGS, EDMA3_CHA_MCASP0_TX,
	                 (struct EDMA3CCPaRAMEntry *)(&EDMA3PaRAM));
	EDMA3Init(SOC_EDMA30CC_0_REGS, 0);
	McASPI2SConfigure(MCASP_TX_MODE, WORD_SIZE, SLOT_SIZE, I2S_SLOTS, MCASP_MODE_DMA);
	EDMA3EnableTransfer(SOC_EDMA30CC_0_REGS, EDMA3_CHA_MCASP0_TX,
							EDMA3_TRIG_MODE_EVENT);
	I2SDataTxRxActivate(MCASP_TX_MODE);
}

void InitSinTable()
{
	int N=4096;
	int i=0;
	for(i=0;i<N;i++)
	{
		sintable[i]=sin(i*pi/4096);  //(0V-2V)
	}
	for(i=0;i<N;i++)
	{
		if(i<2048)
		{
			squaretable[i]=0;
		}
		else
		{
			squaretable[i]=1;
		}
	}
}

static void Delay(volatile unsigned int delay)
{
    while(delay--);
}
