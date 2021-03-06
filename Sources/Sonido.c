/*
 * sonido.c
 *
 *  Created on: 5/06/2017
 *      Author: Krasowski - Madou
 */

#include <stdlib.h>
#include "sonido.h" 
#include <mc9s08sh8.h> 

extern unsigned int NC;
extern unsigned int NC0;

static unsigned int frecuenciaActual=0;
static unsigned int errorDeReproduccion=0;
static unsigned char modoActual=0;
static unsigned int TPM1C1V_aux=0;

/* Funciones privadas */
/* Calcula el valor correspondiente a la frecuencia actual para NC y lo asigna a la variable externa de igual nombre */
void calcular_NC(void);
/* Calcula el valor de NC0 para que en T segundos se oigan todas las frecuencias entre F_MIN y F_MAX, a una resoluci�n de PASO */
void calcular_NC0(unsigned char T);

void SONIDO_init(void){
	SONIDO_apagar();
}

unsigned int SONIDO_getError(void){
	return errorDeReproduccion;
}

void SONIDO_siguiente_f(void){
	frecuenciaActual+=PASO;
	if (frecuenciaActual > F_MAX){
		frecuenciaActual=F_MIN;
	}
	calcular_NC();
}

void SONIDO_prender_mf(unsigned int fa){
	frecuenciaActual=fa;
	calcular_NC();
	modoActual=0;
	TPM1C1V_aux = NC; /* primer NC */
}

void SONIDO_prender_mb(unsigned char T){
	frecuenciaActual=200;
	calcular_NC();
	modoActual=1;
	TPM1C1V_aux = NC; /* primer NC */
	calcular_NC0(T);
}

void SONIDO_apagar(void){
	TPM1C1V_aux = TPM1C1V;
	TPM1C1V = 0;
	TPM1C1SC_CH1IE=0;
	TPM2C0SC_CH0IE=0;
}

void SONIDO_reanudar(void){
	TPM1C1V = TPM1C1V_aux;
	TPM1C1SC_CH1IE=1;
	TPM2C0SC_CH0IE=modoActual;
}


void calcular_NC(void){
	unsigned long auxNC, auxFoc;
	auxNC=8000000UL/(PRESCALER_1*2*frecuenciaActual); /* c�lculo de NC */
	NC=(unsigned int)auxNC;
	auxFoc=8000000UL/(PRESCALER_1*2*auxNC);
	errorDeReproduccion = abs((unsigned int)auxFoc-frecuenciaActual);
}

void calcular_NC0(unsigned char T){
	unsigned long auxNC0;
	unsigned int auxFrec = ((F_MAX-F_MIN)/PASO)/T;
	auxNC0=8000000UL/(PRESCALER_2*auxFrec); /* c�lculo de NC0 */
	NC0=(unsigned int)auxNC0;
}
