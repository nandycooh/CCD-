#include "Filter.h"
#include "math.h"

// LowPassFilter2p 

/*
* @brief  ����IIR�˲�������
* @param  ���ص�IIR_coeff_Typedef�ṹ�壬����Ƶ�ʣ�����
* @note		
* @retval void
* @author across
*/
void cal_iir_coeff(IIR_coeff_Typedef *coeff,float fs, float fc)
{		
  float fr =0;
  float ohm =0;
  float c =0;
	
	if (fc <= 0.0f) {
			// no filtering
			return;
	}
  
  fr= fs/fc;
  ohm=tanf(M_PI_F/fr);
  c=1.0f+2.0f*cosf(M_PI_F/4.0f)*ohm + ohm*ohm;
  
  coeff->fc = fc;
  
  coeff->b0 = ohm*ohm/c;
  coeff->b1 = 2.0f*coeff->b0;
  coeff->b2 = coeff->b0;
  coeff->a1 = 2.0f*(ohm*ohm-1.0f)/c;
  coeff->a2 = (1.0f-2.0f*cosf(M_PI_F/4.0f)*ohm+ohm*ohm)/c;
}

/*
* @brief  �˲�����
* @param  IIR_coeff_Typedef�ṹ�壬���˲���ԭʼ����
* @note		
* @retval �˲��������
* @author across
*/
float get_iir_output(IIR_coeff_Typedef* coeff,float sample)
{
  if (coeff->fc <= 0.0f) {
		// no filtering
    return sample;
  }

	float y_0 = sample - coeff->y_1 * coeff->a1 - coeff->y_2 * coeff->a2;    
	float output = y_0 * coeff->b0 + coeff->y_1 * coeff->b1 + coeff->y_2 * coeff->b2;    
	coeff->y_2 = coeff->y_1;
	coeff->y_1 = y_0;
	return output;
	
}








