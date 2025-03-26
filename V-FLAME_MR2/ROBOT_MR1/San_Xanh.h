////************************ san xanh *********************************
//void ban_giai_doan_hai(void);
//void Xuat_phat(void)
//{
//	RESET_ENCODER();
//	
//	XI_LANH_KEP_VONG_ON;
//	XI_LANH_NANG_HA_ON;
//	
//  robotRun(1750,80);
//	for(i=0;i<50;i++)	while(ENCODER_TONG() < 4000)	{	if(wantExit() == 1) break; vTaskDelay(1);}
//	Bien_nang = 0;
//	noise1 = 0;
//	
//	Resetnangvong = 1;
//	
//	for(i=0;i<50;i++)	while(lazeSauValue > 115)	{			if(wantExit() == 1) break;	
//																											vTaskDelay(1);}		
//	robotRun(1750, 15);																									
//	for(i=0;i<100;i++)	while(CAM_BIEN_TU == 1) {		if(wantExit() == 1) break;	
//																									vTaskDelay(1);}					
//	robotStop(0);
//	vTaskDelay(500);
//		
//	XI_LANH_KEP_VONG_OFF;
//	Resetnangvong = 0;
//																																
//	vTaskDelay(2000);			
//					
//	robotRun(-300, 30);
//	for(i=0;i<100;i++)	while(lazeSauValue < 8)	{	 if(wantExit() == 1) break; vTaskDelay(1);}
//																																																			
//	XI_LANH_NANG_HA_OFF;
//	Bien_nang = 1, noise1 = 0;
//	Cbcc = 1;
//	robotRun(-340 - _robotIMUAngle,30);
//	robotRotate(-900,-0.8,0);
//	for(i=0;i<50;i++)	while(robotAngle() > -860)	{	if(wantExit() == 1) break;	vTaskDelay(1);}	
//	robotRun(-220 - _robotIMUAngle, 23);
//	for(i=0;i<10;i++)	while(lazeTruocValue > 68)	{	if(wantExit() == 1) break;	vTaskDelay(1);}
//	
//	robotStop(0);
//	RESET_ENCODER();
//	//ban ne
//	vTaskDelay(1000);
//	ban_tru();
//	vTaskDelay(1000);
//	
//	//tru gan 2
//	Cbcc = 1;
//	Bien_nang = 1, noise1 = 0;
//	
//	robotRun(850, 60);
//	RESET_ENCODER();
//	for(i=0;i<50;i++)	while(ENCODER_TONG() < 3500)	{	if(wantExit() == 1) break; vTaskDelay(1);}
//	
//	robotRun(850, 18);
//	
//	for(i=0;i<100;i++)	while(lazeTruocValue > 68)	{	 if(wantExit() == 1) break; vTaskDelay(1);}
//	robotStop(0);
//	RESET_ENCODER();
//	vTaskDelay(1000);
//	//ban ne
//	ban_tru();
//	vTaskDelay(1000);
//	
//	//tru gan 3
//	Cbcc = 1;
//	Bien_nang = 1, noise1 = 0;
//	
//	robotRun(850, 60);
//	RESET_ENCODER();
//	for(i=0;i<50;i++)	while(ENCODER_TONG() < 3500)	{	if(wantExit() == 1) break; vTaskDelay(1);}
//	
//	robotRun(850, 18);
//	
//	for(i=0;i<100;i++)	while(lazeTruocValue > 68)	{	 if(wantExit() == 1) break; vTaskDelay(1);}
//	robotStop(0);
//	RESET_ENCODER();
//	vTaskDelay(1000);
//	//ban ne
//	ban_tru();
//	vTaskDelay(1000);
//	
//	
//	//ban tru xa 2 ben minh
//	Bien_nang = 2, noise1 = 0;
//	Cbcc = 1;
//	robotRun(900, 40);
//	for(i=0;i<100;i++)	while(CBQ_CHECK_TUONG == 0)	{	 if(wantExit() == 1) break; vTaskDelay(1);}
//	
//	robotRun(-900 - _robotIMUAngle,50);
//	robotRotate(-1900,-0.8,0);
//	for(i=0;i<50;i++)	while(robotAngle() > -1800)		{	if(wantExit() == 1) break;	vTaskDelay(1);}	
//	
//	robotRotateStop(0);
//	RESET_ENCODER();
//	
//	robotRun(-1100 - _robotIMUAngle,50);
//	
//	for(i=0;i<10;i++)	while(ENCODER_TONG() < 2200 ) 	{ if(wantExit() == 1) break;	vTaskDelay(1);}
//	robotRun(-1050 - _robotIMUAngle,18);
//	// bat them 1 lan tru
//	for(i=0;i<10;i++)	while(lazeTruocValue > 360 ) 	{ if(wantExit() == 1) break;	vTaskDelay(1);}
//	robotStop(0);
//	vTaskDelay(3000);
//	//ban ne
//	ban_tru();
//	RESET_ENCODER();
//	vTaskDelay(1000);
//	
//	// ban tru xa ben doi phuong
//	
//	Bien_nang = 3, noise1 = 0;
//	Cbcc = 1;
//	robotRun(900,35);
//	for(i=0;i<10;i++)	while(CBQ_CHECK_TUONG == 0) 	{ if(wantExit() == 1) break;	vTaskDelay(1);}
//	
//	RESET_ENCODER();
//	robotRun(1250,20);
//	robotRotate(-1580,1,0);
//	for(i=0;i<50;i++)	while(ENCODER_TONG() < 200)	{	if(wantExit() == 1) break; vTaskDelay(1);}
//	
//	robotStop(0);
//	
//	
////	robotRotate(900,0.8,0);
////	for(i=0;i<50;i++)	while(robotAngle() < -1700)	{	if(wantExit() == 1) break;	vTaskDelay(1);}
//	robotRotate(900,0.1,0);
//	for(i=0;i<10;i++)	while(lazeTruocValue > 340) 	{ if(wantExit() == 1) break;	vTaskDelay(1);}
//	robotStop(0);
//	vTaskDelay(2500);
//	ban_tru();
//	vTaskDelay(1000);
//	
//	robotStop(0);
//	SEND_UART(1,'a');
//	vTaskDelay(100);
//	Vi_tri = 1;
//	return;
//}
//void xoayTruXaDoiPhuong(void){
//	Bien_nang = 4, noise1 = 0;
//	Cbcc = 1;
//	robotRotate(900,0.8,0);
//	for(i=0;i<50;i++)	while(robotAngle() < 250)	{	if(wantExit() == 1) break;	vTaskDelay(1);}
//	robotRotate(900,0.1,0);
//	for(i=0;i<10;i++)	while(lazeTruocValue > 380) 	{ if(wantExit() == 1) break;	vTaskDelay(1);}
//	robotStop(0);
//	vTaskDelay(1000);
////	for(i=0;i<10;i++) while(1){ if(choBanTay() == 1) break;	vTaskDelay(1);};
////	ban_tru();
////	//cho ban tay
////	vTaskDelay(1000);
//	Vitri_tru = 1;
//	
//	robotStop(0);
//	return;
//}

//void xoayTruXaHai(void){
//	Bien_nang = 5, noise1 = 0;
//	Cbcc = 1;
//	robotRotate(-900,-0.8,0);
//	for(i=0;i<50;i++)	while(robotAngle() > -350)	{	if(wantExit() == 1) break;	vTaskDelay(1);}
//	robotRotate(-900,-0.1,0);
//	for(i=0;i<10;i++)	while(lazeTruocValue > 360) 	{ if(wantExit() == 1) break;	vTaskDelay(1);}
//	robotStop(0);
//	vTaskDelay(1000);
////	for(i=0;i<10;i++) while(1){ if(choBanTay() == 1) break;	vTaskDelay(1);};
////	ban_tru();
////	//cho ban tay
////	vTaskDelay(1000);
//	Vitri_tru = -1;
//	robotStop(0);
//	return;
//}
//void xoayTruXaBon(void){
//	if(Vitri_tru == 1){
//		Bien_nang = 3, noise1 = 0;
//		Cbcc = 1;
//		robotRotate(-900,-0.8,0);
//		for(i=0;i<50;i++)	while(robotAngle() > 100)	{	if(wantExit() == 1) break;	vTaskDelay(1);}
//		robotRotate(-900,-0.1,0);
//		for(i=0;i<10;i++)	while(lazeTruocValue > 360) 	{ if(wantExit() == 1) break;	vTaskDelay(1);}
//		robotStop(0);
//		vTaskDelay(1000);
////		for(i=0;i<10;i++) while(1){ if(choBanTay() == 1) break;	vTaskDelay(1);};
////		ban_tru();
////		//cho ban tay
////		vTaskDelay(1000);
//		Vitri_tru = 0;
//		
//		robotStop(0);
//		SEND_UART(1,'a');
//		return;
//	}
//	if(Vitri_tru == -1){
//		Bien_nang = 3, noise1 = 0;
//		Cbcc = 1;
//		robotRotate(900,0.8,0);
//		for(i=0;i<50;i++)	while(robotAngle() < -100 )	{	if(wantExit() == 1) break;	vTaskDelay(1);}
//		robotRotate(900,0.1,0);
//		for(i=0;i<10;i++)	while(lazeTruocValue > 360) 	{ if(wantExit() == 1) break;	vTaskDelay(1);}
//		robotStop(0);
//		vTaskDelay(1000);
////		for(i=0;i<10;i++) while(1){ if(choBanTay() == 1) break;	vTaskDelay(1);};
////		ban_tru();
////		//cho ban tay
////		vTaskDelay(1000);
//		Vitri_tru = 0;
//		
//		robotStop(0);
//		SEND_UART(1,'a');
//		
//		return;
//	}
//}

//void layVongLanHai(void){
//	if(Vitri_tru == 1){
//		robotRotate(-900,-0.8,0);
//		for(i=0;i<50;i++)	while(robotAngle() > 100)	{	if(wantExit() == 1) break;	vTaskDelay(1);}
//		robotRotate(-900,-0.1,0);
//		for(i=0;i<10;i++)	while(lazeTruocValue > 360) 	{ if(wantExit() == 1) break;	vTaskDelay(1);}
//		robotStop(0);
//		vTaskDelay(1000);
//		
//	}
//	if(Vitri_tru == -1){
//		robotRotate(900,0.8,0);
//		for(i=0;i<50;i++)	while(robotAngle() < -100 )	{	if(wantExit() == 1) break;	vTaskDelay(1);}
//		robotRotate(900,0.1,0);
//		for(i=0;i<10;i++)	while(lazeTruocValue > 360) 	{ if(wantExit() == 1) break;	vTaskDelay(1);}
//		robotStop(0);
//		vTaskDelay(1000);
//	}
//	SEND_UART(1,'a');
//	vTaskDelay(100);
//	RESET_ENCODER();
//	robotRun(1900,60);
//	for(i=0;i<50;i++)	while(ENCODER_TONG() < 200)	{	if(wantExit() == 1) break; vTaskDelay(1);}
//	robotRotate(900,0.8,0);
//	for(i=0;i<50;i++)	while(robotAngle() < 600)	{	if(wantExit() == 1) break;	vTaskDelay(1);}
//	robotRotateStop(0);
//	
//	robotRun(1750,60);
//	for(i=0;i<50;i++)	while(ENCODER_TONG() < 2000)	{	if(wantExit() == 1) break; vTaskDelay(1);}
//	XI_LANH_KEP_VONG_ON;
//	XI_LANH_NANG_HA_ON;
//	robotRun(1750,60);
//	for(i=0;i<50;i++)	while(ENCODER_TONG() < 6000)	{	if(wantExit() == 1) break; vTaskDelay(1);}
//	Bien_nang = 0;
//	noise1 = 0;
//	
//	Resetnangvong = 1;
//	
//	for(i=0;i<50;i++)	while(lazeSauValue > 120)	{			if(wantExit() == 1) break;	
//																											vTaskDelay(1);}		
//	robotRun(1750, 15);																									
//	for(i=0;i<100;i++)	while(CAM_BIEN_TU == 1) {		if(wantExit() == 1) break;	
//																									vTaskDelay(1);}					
//	robotStop(0);
//	vTaskDelay(500);
//		
//	XI_LANH_KEP_VONG_OFF;
//	Resetnangvong = 0;																						
//	vTaskDelay(2000);			
//	RESET_ENCODER();
//	robotRun(-300, 30);
//	for(i=0;i<100;i++)	while(lazeSauValue < 8)	{	 if(wantExit() == 1) break; vTaskDelay(1);}
//																																																			
//	XI_LANH_NANG_HA_OFF;
//	robotRun(-230, 60);
//	for(i=0;i<50;i++)	while(ENCODER_TONG() < 1800)	{	if(wantExit() == 1) break; vTaskDelay(1);}

//	
//	robotRotateStop(0);
//	robotRun(-340,25);
//	robotRotate(-900,-0.8,0);
//	for(i=0;i<50;i++)	while(robotAngle() > 0)	{	if(wantExit() == 1) break;	vTaskDelay(1);}
//	robotRun(-220 - _robotIMUAngle, 23);
//	for(i=0;i<50;i++)	while(ENCODER_TONG() < 3500)	{	if(wantExit() == 1) break; vTaskDelay(1);}
//	robotStop(0);
//	RESET_ENCODER();
//	robotRun(850,50);
//	for(i=0;i<50;i++)	while(ENCODER_TONG() < 1800)	{	if(wantExit() == 1) break; vTaskDelay(1);}
//	
//	Bien_nang = 3, noise1 = 0;
//	Cbcc = 1;
//	robotRun(900,35);
//	for(i=0;i<10;i++)	while(CBQ_CHECK_TUONG == 0) 	{ if(wantExit() == 1) break;	vTaskDelay(1);}
//	
//	RESET_ENCODER();
//	robotRun(-1250,20);
//	robotRotate(0,0.9,0);
//	for(i=0;i<50;i++)	while(ENCODER_TONG() < 200)	{	if(wantExit() == 1) break; vTaskDelay(1);}
//	robotStop(0);
//	
//	robotRotate(900,0.1,0);
//	for(i=0;i<10;i++)	while(lazeTruocValue > 340) 	{ if(wantExit() == 1) break;	vTaskDelay(1);}
//	robotStop(0);
//	vTaskDelay(2000);
//	for(i=0;i<10;i++) while(1){ if(choBanTay() == 1) break;	vTaskDelay(1);};
//	ban_tru();
//	vTaskDelay(2000);
//	SEND_UART(1,'a');
//	Vitri_tru = 0;
////	robotRotate(900,0.8,0);
////	for(i=0;i<50;i++)	while(robotAngle() < -1600)	{	if(wantExit() == 1) break;	vTaskDelay(1);}
////	robotRotate(900,0.1,0);
////	for(i=0;i<10;i++)	while(lazeTruocValue > 340) 	{ if(wantExit() == 1) break;	vTaskDelay(1);}
//	
//	return;
//}
