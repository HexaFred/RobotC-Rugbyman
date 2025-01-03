#pragma config(Sensor, S1,     tactileAv,      sensorTouch)
#pragma config(Sensor, S2,     tactileAr,      sensorTouch)
#pragma config(Sensor, S3,     captLumi,       sensorLightActive)
#pragma config(Sensor, S4,     ultraSon,       sensorSONAR)
#pragma config(Motor,  motorA,          mPince,        tmotorNXT, PIDControl, encoder)
#pragma config(Motor,  motorB,          mDroit,        tmotorNXT, PIDControl, encoder)
#pragma config(Motor,  motorC,          mGauche,       tmotorNXT, PIDControl, encoder)

#define diamRoue 5.6 /*cm*/
#define entraxes 11.2 /*cm*/

#define avant 1
#define arriere -1
#define gauche 1
#define droite -1

#define distL1 31 /*cm*/
#define distL2 16 /*cm*/
#define distL3 24 /*cm*/
#define distPtBleu 80 /*cm*/
#define distZoneBleu 155 /*cm*/

void tourner2roues(int direction, int angle, int puissance){
	int nbDeg;
	if (direction==droite){
		puissance=puissance*(-1);
	}
	if ((direction!=droite) && (direction!=gauche)){
		angle=0;
		}
	nMotorEncoder[mDroit]=0;
	nbDeg=(angle*entraxes)/(diamRoue);
	motor[mDroit]=puissance;
	motor[mGauche]=-puissance;
	if (direction==gauche){
		while (nMotorEncoder[mDroit]<nbDeg){}
	}
	if (direction==droite){
		while (nMotorEncoder[mDroit]>(-nbDeg)){}
	}
	motor[mDroit]=0;
	motor[mGauche]=0;
}

void avancerDistance(int sens, int distance, int puissance){

	int nbDeg;
	nbDeg=(360*distance)/(PI*diamRoue);
	puissance=puissance*sens;
	motor[mDroit]=puissance;
	motor[mGauche]=puissance;
	nMotorEncoder[mDroit]=0;
	if (sens==avant){
		while(nMotorEncoder[mDroit]<nbDeg){}
	}
	if (sens==arriere){
		while(nMotorEncoder[mDroit]>-nbDeg){}
	}
	motor[mDroit]=0;
	motor[mGauche]=0;
}

void avancerJusquaLigne( int vitesse, int seuil){

	motor[mDroit]=vitesse;
	motor[mGauche]=vitesse;
	while(SensorValue(captLumi)>seuil){}
	motor[mDroit]=0;
	motor[mGauche]=0;
}

int calculSeuil(){
	int seuil;
	nxtDisplayCenteredTextLine(3,"Couleur sombre");

	while (nNxtButtonPressed!=kEnterButton){}
	seuil=SensorValue(captLumi);

	while (nNxtButtonPressed==kEnterButton){}
	nxtDisplayCenteredTextLine(3,"Couleur clair");

	while (nNxtButtonPressed!=kEnterButton){}
	seuil=seuil+SensorValue(captLumi);

	while (nNxtButtonPressed==kEnterButton){}
	nxtDisplayCenteredTextLine(3,"Confirmer");

	while (nNxtButtonPressed!=kEnterButton){}
	while (nNxtButtonPressed==kEnterButton){}
	eraseDisplay();
	return(seuil / 2);
	}

void ouvrirPince(int deg, int puissance){
	if (deg>40){
		deg=40;}
	nMotorEncoder[mPince]=0;
	motor[mPince]=puissance;
	while(nMotorEncoder[mPince]<deg){}
	motor[mPince]=0;
}

void fermerPince(int deg, int puissance){
	if (deg>40){
		deg=40;}
	nMotorEncoder[mPince]=0;
	motor[mPince]=-puissance;
	while(nMotorEncoder[mPince]>-deg){}
	motor[mPince]=0;
}

task main()
{
	int seuilNoir, seuilBleu, nbLigne, degDistPtBleu,degZoneBleu ;
	bool choixZ1 ;


	nxtDisplayCenteredTextLine(3,"Seuil noir");
	while (nNxtButtonPressed!=kEnterButton){}
	while (nNxtButtonPressed==kEnterButton){}
	seuilNoir = calculSeuil();
	nxtDisplayCenteredTextLine(3,"Seuil bleue");
	while (nNxtButtonPressed!=kEnterButton){}
	while (nNxtButtonPressed==kEnterButton){}
	seuilBleu = calculSeuil();


	/* Placer le robot dans ZD */

	avancerJusquaLigne(30,seuilNoir);
	tourner2roues(gauche, 90, 10);
	motor[mDroit] = 30;
	motor[mGauche] = 30;
	while (SensorValue(captLumi)>seuilNoir){}





	avancerDistance(avant,20,30);
	nxtDisplayCenteredTextLine(3,"1  |  2  |  3");
	while ((nNxtButtonPressed!=kLeftButton) && (nNxtButtonPressed!=kEnterButton) && (nNxtButtonPressed!=kRightButton) ){}
	if (nNxtButtonPressed==kLeftButton){
		nbLigne=1;
	}
	if (nNxtButtonPressed==kEnterButton){
		nbLigne=2;
	}
	if (nNxtButtonPressed==kRightButton){
		nbLigne=3;
	}
	eraseDisplay();
	nxtDisplayCenteredTextLine(3,"Confirmer");
	while (nNxtButtonPressed!=kEnterButton){}
	while (nNxtButtonPressed==kEnterButton){}

	/* Retourner le robot dans ZN */

	nxtDisplayCenteredTextLine(3,"Demarrer");
	while (nNxtButtonPressed!=kEnterButton){}
	while (nNxtButtonPressed==kEnterButton){}
	eraseDisplay();
	avancerDistance(avant, distL1, 30);
	if (nbLigne >= 2){
		avancerDistance(avant, distL2, 30);
	}
	if (nbLigne == 3){
		avancerDistance(avant, distL3, 30);
	}
	motor[mDroit] = 0;
	motor[mGauche] = 0;
	tourner2roues(droite, 90, 10);
	ouvrirPince(30,30);
	nMotorEncoder[mDroit] = 0;
	degDistPtBleu = (distPtBleu * 360) / (PI * diamRoue);
	motor[mDroit] = 30;
	motor[mGauche] = 30;
	while (SensorValue(tactileAv)==0){}
	if (nMotorEncoder[mDroit] < degDistPtBleu){
		choixZ1 = true;
	}
	else {
		choixZ1 = false;
	}
	motor[mDroit] = 0;
	motor[mGauche] = 0;
	fermerPince(30,30);
	motor[mDroit] = 30;
	motor[mGauche] = 30;
	degZoneBleu = (distZoneBleu * 360) / (PI * diamRoue);
	while (nMotorEncoder[mDroit] < degZoneBleu){}
	if (choixZ1){
		tourner2roues(droite,90,10);
	}
	else {
		tourner2roues(gauche,90,10);
	}
	avancerJusquaLigne(30,seuilBleu);
	ouvrirPince(30,30);
	wait1Msec(2000);
	fermerPince(30,30);









}
