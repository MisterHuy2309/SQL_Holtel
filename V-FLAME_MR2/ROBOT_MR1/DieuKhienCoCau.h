//*****************************8 bien toan cuc *************************************
char 	Khoang_cach_1_vong = 16, bien_xl_nang_ha, bien_xl_mac_vong, bien_xl_chot, bien_xl_kep_vong, bien_xl_day,bien_xl_tru_giua,bien_xl_keo_loxo;
char	vi_tri = 0, Cbcc = 0, Cbnv = 0,Cbnn = 0, Resetnangvong = 0;

int  Nang_vong_max = 322, Nang_vong_min = 105, i; 
int  Nang_goc_max = 540, Nang_goc_min = 300;

int en_tru = 60;

void robotGamePadControl(void)
{
	
    int tocdo = 20,td_x = 7;
		float	tocdoXoay = 0;
    int rJoy_LR  = (RJOY_LR >= 5 && RJOY_LR <= 250)? 1 : 0; 
    int lJoy_LR  = (LJOY_LR >= 5 && LJOY_LR <= 250)? 1 : 0;

    if(UP && DOWN && RIGHT && LEFT && rJoy_LR && lJoy_LR && O)
		{ // Khong dieu khien
        robotStop(0); //truot banh
				
        return;
    }                
/*_____________________________//___________________________*/

/********************** TOC DO ****************************/
/*_____________________________//___________________________*/
     if(!L1) tocdo = 35, tocdoXoay = 0.4;
		 else tocdo = 10, tocdoXoay = 0.2;
    
    //--------- chay cac huong -----------
    if		(!UP && DOWN && RIGHT && LEFT && L2)   	robotRun(0, tocdo);
    else if(UP && !DOWN && RIGHT && LEFT && L2)   robotRun(1800, tocdo);
    else if(UP && DOWN && !RIGHT && LEFT && L2)   robotRun(900, tocdo);
    else if(UP && DOWN && RIGHT && !LEFT && L2)   robotRun(-900, tocdo);
    
    else if(!UP && DOWN && !RIGHT && LEFT && L2)  robotRun(450,tocdo);
    else if(!UP && DOWN && RIGHT && !LEFT && L2)  robotRun(-450,tocdo);
    else if(UP && !DOWN && !RIGHT && LEFT && L2)  	robotRun(1350,tocdo);
    else if(UP && !DOWN && RIGHT && !LEFT && L2)  robotRun(-1350,tocdo);
	
    //-------------- Dang chay va Khong chay nua, chi xoay ----------------
    if(UP && DOWN && RIGHT && LEFT && !rJoy_LR && lJoy_LR  && robotIsRun()) robotStop(0);   

    //-------------- Xoay ----------------
    if(rJoy_LR) robotRotateStop();
    else if((RJOY_LR < 5) && L2)
    {
        if(robotIsRun()) robotRotateFree(-0.5,0);
        else robotRotateFree(-tocdoXoay,0);
    }
    else if((RJOY_LR > 250) && L2)
    {
        if(robotIsRun()) robotRotateFree(0.5,0);
        else robotRotateFree(tocdoXoay,0);
    }

    else robotRotateStop();
	}
//////-------------------------------------------------------------	
	void ADCValue_Control(void)
{
	//KHAI BAO LAZER
	static unsigned int lazeTruocCouter = 0, lazeTruoc_SUM = 0;
	static unsigned int lazePhaiCouter = 0, lazePhai_SUM = 0;
	static unsigned int lazeTraiCouter = 0, lazeTrai_SUM = 0;
	
	//KHAI BAO BIEN TRO
	
	static unsigned int BientroTrucXCouter = 0,BientroTrucX_SUM = 0; 
	static unsigned int BientroTrucYCouter = 0,BientroTrucY_SUM = 0; 
	static unsigned int BientroMamXoayCouter= 0,BientroMamXoay_SUM = 0;
////------------------------	
	if(lazeTruocCouter++ < 50)	
	{
		lazeTruoc_SUM += cam_bien_laze_truoc;  
	}
	else
  {
		lazeTruocValue = lazeTruoc_SUM/50;
		lazeTruocCouter = 0;
		lazeTruoc_SUM = 0;}
	
	if(lazePhaiCouter++ < 50)	
	{
		lazePhai_SUM += cam_bien_laze_phai;  
	}
	else
  {
		lazePhaiValue = lazePhai_SUM/50;
		lazePhaiCouter = 0;
		lazePhai_SUM = 0;}
	
	if(lazeTraiCouter++ < 50)	
	{
		lazeTrai_SUM += cam_bien_laze_trai;  
	}
	else
  {
		lazeTraiValue = lazeTrai_SUM/50;
		lazeTraiCouter = 0;
		lazeTrai_SUM = 0;
	}
	
	// BIEN TRO
	if(BientroTrucXCouter++ < 50)	
	{
		BientroTrucX_SUM += bien_tro_nang_trucX;
	}
	else
  {
		BienTroTrucXValue = BientroTrucX_SUM/50;
		BientroTrucXCouter = 0;
		BientroTrucX_SUM = 0;
	}
	
	if(BientroTrucYCouter++ < 50)	
	{
		BientroTrucY_SUM += bien_tro_nang_trucY;
	}
	else
  {
		BienTroTrucYValue = BientroTrucY_SUM/50;
		BientroTrucYCouter = 0;
		BientroTrucY_SUM = 0;
	}
	
	if(BientroMamXoayCouter++ < 50)	
	{
		BientroMamXoay_SUM += bien_tro_xoay_mam;
	}
	else
  {
		BienTroMamXoayValue = BientroMamXoay_SUM/50;
		BientroMamXoayCouter = 0;
		BientroMamXoay_SUM = 0;
	}
}
void test_truc_XY (void)
{
	if(!X) 
	{
		XI_LANH_NANG_BONG_ON;
		XI_LANH_THA_BONG_ON;
		
	}
	if(!SQUARE)
	{
		XI_LANH_THA_BONG_OFF;
		  XI_LANH_NANG_BONG_OFF;
	}
	
}