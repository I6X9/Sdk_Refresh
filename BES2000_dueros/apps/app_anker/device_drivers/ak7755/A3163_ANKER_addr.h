#ifndef _AK77XX_DSP_ADDR_INCLUDED
#define _AK77XX_DSP_ADDR_INCLUDED

// Component: Volume (fader_mul2_1)
#define 	CRAM_ADDR_LEVEL_Volume 	0x0000
#define 	CRAM_ADDR_ATT_TIME_Volume 	0x0001
#define 	CRAM_ADDR_REL_TIME_Volume 	0x0003

// Component: HeardRoom (fader_mul2_2)
#define 	CRAM_ADDR_LEVEL_HeardRoom 	0x0006
#define 	CRAM_ADDR_ATT_TIME_HeardRoom 	0x0007
#define 	CRAM_ADDR_REL_TIME_HeardRoom 	0x0009

// Component: GEQs_2 (biquad2_1)
#define 	CRAM_ADDR_BAND1_GEQs_2 	0x000C
#define 	CRAM_ADDR_BAND2_GEQs_2 	0x0016
#define 	CRAM_ADDR_BAND3_GEQs_2 	0x0020
#define 	CRAM_ADDR_BAND4_GEQs_2 	0x002A
#define 	CRAM_ADDR_BAND5_GEQs_2 	0x0034
#define 	CRAM_ADDR_BAND6_GEQs_2 	0x0039
#define 	CRAM_ADDR_BAND7_GEQs_2 	0x003E
#define 	CRAM_ADDR_BAND8_GEQs_2 	0x0043
#define 	CRAM_ADDR_BAND9_GEQs_2 	0x0048

// Component: GEQs_1 (biquad2_2)
#define 	CRAM_ADDR_BAND1_GEQs_1 	0x004D
#define 	CRAM_ADDR_BAND2_GEQs_1 	0x0057
#define 	CRAM_ADDR_BAND3_GEQs_1 	0x0061

// Component: DRC2_1 (u_DRC2_1)
#define 	CRAM_ADDR_BYPASS_DRC2_1 	0x006B
#define 	CRAM_ADDR_THRESHOLD_DRC2_1 	0x006C
#define 	CRAM_ADDR_SOFT_KNEE_DRC2_1 	0x006E
#define 	CRAM_ADDR_ATT_SPEED_DRC2_1 	0x006F
#define 	CRAM_ADDR_REL_SPEED_DRC2_1 	0x0071

// Component: GEQs_3 (biquad2_3)
#define 	CRAM_ADDR_BAND1_GEQs_3 	0x0073
#define 	CRAM_ADDR_BAND2_GEQs_3 	0x007D
#define 	CRAM_ADDR_BAND3_GEQs_3 	0x0082
#define 	CRAM_ADDR_BAND4_GEQs_3 	0x008C
#define 	CRAM_ADDR_BAND5_GEQs_3 	0x0096
#define 	CRAM_ADDR_BAND6_GEQs_3 	0x00A0
#define 	CRAM_ADDR_BAND7_GEQs_3 	0x00AA
#define 	CRAM_ADDR_BAND8_GEQs_3 	0x00AF
#define 	CRAM_ADDR_BAND9_GEQs_3 	0x00B4
#define 	CRAM_ADDR_BAND10_GEQs_3 	0x00B9

// Component: DRC2_2 (u_DRC2_2)
#define 	CRAM_ADDR_BYPASS_DRC2_2 	0x00BE
#define 	CRAM_ADDR_THRESHOLD_DRC2_2 	0x00BF
#define 	CRAM_ADDR_SOFT_KNEE_DRC2_2 	0x00C1
#define 	CRAM_ADDR_ATT_SPEED_DRC2_2 	0x00C2
#define 	CRAM_ADDR_REL_SPEED_DRC2_2 	0x00C4

// Component: MIXER2_1 (mixerl2_1)
#define 	CRAM_ADDR_VOL_IN1_MIXER2_1 	0x00C6
#define 	CRAM_ADDR_VOL_IN2_MIXER2_1 	0x00C7

// Component: MIXER2_2 (mixerl2_2)
#define 	CRAM_ADDR_VOL_IN1_MIXER2_2 	0x00C8
#define 	CRAM_ADDR_VOL_IN2_MIXER2_2 	0x00C9

// Component: DRC2_3 (u_DRC2_3)
#define 	CRAM_ADDR_BYPASS_DRC2_3 	0x00CA
#define 	CRAM_ADDR_THRESHOLD_DRC2_3 	0x00CB
#define 	CRAM_ADDR_SOFT_KNEE_DRC2_3 	0x00CD
#define 	CRAM_ADDR_ATT_SPEED_DRC2_3 	0x00CE
#define 	CRAM_ADDR_REL_SPEED_DRC2_3 	0x00D0

// Component: GEQs_4 (biquad2_4)
#define 	CRAM_ADDR_BAND1_GEQs_4 	0x00D2
#define 	CRAM_ADDR_BAND2_GEQs_4 	0x00DC
#define 	CRAM_ADDR_BAND3_GEQs_4 	0x00E1
#define 	CRAM_ADDR_BAND4_GEQs_4 	0x00EB
#define 	CRAM_ADDR_BAND5_GEQs_4 	0x00F5
#define 	CRAM_ADDR_BAND6_GEQs_4 	0x00FA

// Component: SFADER_2 (fader_mul2_3)
#define 	CRAM_ADDR_LEVEL_SFADER_2 	0x00FF
#define 	CRAM_ADDR_ATT_TIME_SFADER_2 	0x0100
#define 	CRAM_ADDR_REL_TIME_SFADER_2 	0x0102

// Component: SFADER_1 (fader_mul2_4)
#define 	CRAM_ADDR_LEVEL_SFADER_1 	0x0105
#define 	CRAM_ADDR_ATT_TIME_SFADER_1 	0x0106
#define 	CRAM_ADDR_REL_TIME_SFADER_1 	0x0108

// Component: Limiter2_1 (limiter2_1)
#define 	CRAM_ADDR_ATT_TIME_Limiter2_1 	0x010D
#define 	CRAM_ADDR_REL_TIME_Limiter2_1 	0x010F
#define 	CRAM_ADDR_THRESHOLD_Limiter2_1 	0x0112

#endif //#end of _AK77XX_DSP_ADDR_INCLUDED