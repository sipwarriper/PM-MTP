/*
Entrada: Nom del fitxer dels enviaments
Sortida: Fase I de l'informe de ventes, enviaments i ingresos del 2014 de l'empresa Video-Games Springfield
*/


#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

using namespace std;

const string SHIPMENTS_2014 = "shipments_2014_06am.mtp"; // Fitxer d’enviaments acabat (versió 6 del matí)
const string US_CENSUS_2014 = "census_2014.usa"; // Fitxer del cens estatal
const int US_POPULATION = 318857056; //Poblacio total d'USA
const int US_STATES = 51; // Registres del cens estàndard d’USA: 50 estats + 1 districte
const int QUARTERS = 4; // 1 "Quarter" representa un trimestre d’un any (4 "quarters" x 3 mesos = 12 mesos)
const int WIDTH_STATE_NAME = 21; // Amplada de camp per output nom d’estats
const int WIDTH_STATE_CODE = 3; // Amplada de camp per output codi estats
const int WIDTH = 17; // Amplada de camp estàndard per outputs genèrics
const int RATE_PRECISION = 3; // Precisió decimal per ratis
const int USD_PRECISION = 2; // Precisió decimal per imports USD (dòlar d’US)

struct Estat
{
	//struct que s'utilitzara per guardar la info de cada estat.
	string nom;
	string codi;
	unsigned int poblacio;
	unsigned int unitats[5];
	unsigned int enviaments[5];
	double preu[5];
};

void inicialitzar(Estat dades[], Estat & estatsUnits) {
	/*
	Pre: -
	Post: dades i estatsUntis amb les taules enviaments, preu i unitats amb tots els elements incialitzats i estatsUnits amb el codi, el nom i la poblacio.
	*/
	for (int i = 0; i < US_STATES;i++) {
		for (int j = 0; j < 5;j++) {
			dades[i].enviaments[j] = 0;
			dades[i].preu[j] = 0;
			dades[i].unitats[j] = 0;
			estatsUnits.enviaments[j] = 0;
			estatsUnits.preu[j] = 0;
			estatsUnits.unitats[j] = 0;
		}
	}
	estatsUnits.codi = "US";
	estatsUnits.nom = "United States";
	estatsUnits.poblacio = US_POPULATION;

}

void sumTotals(Estat dades[], Estat & estatsUnits) {
	/*
	Pre: el primer element de enviaments, preu i unitats = 0
	Post: enviaments, preu i unitats totals calculades i colocades al element 0 de les corresponents taules
	*/
	for (int i = 0; i < US_STATES;i++) {
		dades[i].enviaments[0] = dades[i].enviaments[1] + dades[i].enviaments[2] + dades[i].enviaments[3] + dades[i].enviaments[4];
		dades[i].preu[0] = dades[i].preu[1] + dades[i].preu[2] + dades[i].preu[3] + dades[i].preu[4];
		dades[i].unitats[0] = dades[i].unitats[1] + dades[i].unitats[2] + dades[i].unitats[3] + dades[i].unitats[4];
	}
	estatsUnits.enviaments[0] = estatsUnits.enviaments[1] + estatsUnits.enviaments[2] + estatsUnits.enviaments[3] + estatsUnits.enviaments[4];
	estatsUnits.preu[0] = estatsUnits.preu[1] + estatsUnits.preu[2] + estatsUnits.preu[3] + estatsUnits.preu[4];
	estatsUnits.unitats[0] = estatsUnits.unitats[1] + estatsUnits.unitats[2] + estatsUnits.unitats[3] + estatsUnits.unitats[4];
}

void ordenar(Estat dades[], int n, char tipus) {
	/*
	Pre: n=longitud de la taula i tipus = 'S','A','B' o 'C'
	Post: dades ordenada en funcio de la ordenacio desitjada {
	si tipus es S, ordenada alfabeticament pel codi de l'estat
	si tipus es A, ordenada de major a menor per preu
	si tipus es B, ordenada de major a menor per unitats
	si el tipus es C, ordenada de major a menor per enviaments
	*/
	Estat k;
	for (int i = 0; i < n; i++)
	{
		for (int j = (n - 1); j > i; j--)
		{
			if (tipus == 'S') {
				if (dades[j].codi < dades[j - 1].codi)
				{
					k = dades[j];
					dades[j] = dades[j - 1];
					dades[j - 1] = k;
				}
			}
			else if (tipus == 'A') {
				if (dades[j].preu[0] > dades[j - 1].preu[0])
				{
					k = dades[j];
					dades[j] = dades[j - 1];
					dades[j - 1] = k;
				}
				else if (dades[j].preu[0] == dades[j - 1].preu[0]) {
					if (dades[j].nom < dades[j - 1].nom)
					{
						k = dades[j];
						dades[j] = dades[j - 1];
						dades[j - 1] = k;
					}
				}
			}
			else if (tipus == 'B') {
				if (dades[j].unitats[0] > dades[j - 1].unitats[0])
				{
					k = dades[j];
					dades[j] = dades[j - 1];
					dades[j - 1] = k;
				}
				else if (dades[j].unitats[0] == dades[j - 1].unitats[0]) {
					if (dades[j].nom < dades[j - 1].nom)
					{
						k = dades[j];
						dades[j] = dades[j - 1];
						dades[j - 1] = k;
					}
				}
			}
			else if (tipus == 'C') {
				if (dades[j].enviaments[0] > dades[j - 1].enviaments[0])
				{
					k = dades[j];
					dades[j] = dades[j - 1];
					dades[j - 1] = k;
				}
				else if (dades[j].enviaments[0] == dades[j - 1].enviaments[0]) {
					if (dades[j].nom < dades[j - 1].nom)
					{
						k = dades[j];
						dades[j] = dades[j - 1];
						dades[j - 1] = k;
					}
				}
			}
		}
	}

}
void llegirCens(ifstream & cens, Estat dades[]) {
	/*
	Pre: taula de estats buida
	Post: taula de estats amb els elements llegits del fitxer i ordenada alfabeticament
	*/
	for (int i = 0; i<US_STATES; i++)
	{
		cens >> dades[i].nom;
		cens >> dades[i].codi;
		cens >> dades[i].poblacio;
		ordenar(dades, i, 'S');
	}
}
void escruiureCens(Estat dades[]) {
	/*
	Pre:-
	Post: elements del cens de la taula (dades) escrits
	*/
	cout << left << setw(WIDTH_STATE_CODE) << "PC" << setw(WIDTH_STATE_NAME) << "State " << setw(WIDTH) << right << "Population" << endl;
	cout << left << setw(WIDTH_STATE_CODE) << "US" << setw(WIDTH_STATE_NAME) << "United States " << setw(WIDTH) << right << US_POPULATION << right << endl;
	for (int i = 0; i<US_STATES; i++)
	{
		cout << left << setw(WIDTH_STATE_CODE) << dades[i].codi << setw(WIDTH_STATE_NAME) << dades[i].nom << setw(WIDTH) << right << dades[i].poblacio << right << endl;
	}

	cout << endl << "=====[ Report S: Census states <End> ]=====" << endl << endl;

}

int cercar(Estat dades[], string a) {
	/*
	Pre:-
	Post: Posicio on es troba a o -1 si no hi es
	*/
	bool trobat = false;
	int k = 0;
	while ((!trobat) && (k<51)) {
		trobat = dades[k].codi == a;
		k++;
	}
	if (!trobat) k = -1;
	else k -= 1;
	return (k);
}

void llegirUnitatsIPreus(ifstream & shipments, Estat dades[], Estat & estatsUnits, int k, int pos) {
	/*
	Pre: k=posicio de la taula on volem escriure i pos = posicio dintre de l'Estat on volem escriure
	Post: Taula actulitzada amb els nous elements (enviaments, unitats i preu)
	*/
	unsigned int temporal;
	double diners;
	dades[k].enviaments[pos]++;
	estatsUnits.enviaments[pos]++;
	shipments >> temporal;
	dades[k].unitats[pos] += temporal;
	estatsUnits.unitats[pos] += temporal;
	shipments >> diners;
	dades[k].preu[pos] += diners;
	estatsUnits.preu[pos] += diners;
}

int detTrimestre(int data) {
	/*
	Pre: data enter de 4 digits del format mmdd
	Post: trimestre al qual correspon la data
	*/
	int trimestre;
	if (data >= 101 && data <= 331) trimestre = 1;
	else if (data >= 401 && data <= 630) trimestre = 2;
	else if (data >= 701 && data <= 930) trimestre = 3;
	else if (data >= 1001 && data <= 1231) trimestre = 4;
	return trimestre;
}

void reportA(Estat dades[]) {
	/*
	Pre:-
	Post: Escriu el report A per pantalla
	*/
	double rate = 0;

	ordenar(dades, US_STATES, 'A');
	for (int i = 0; i < US_STATES; i++)
	{
		if (dades[i].preu[0] != 0) {
			rate = (double(dades[i].poblacio) / dades[i].preu[0]);
		}
		else rate = 0;
		cout << setw(WIDTH_STATE_NAME) << left << dades[i].nom << setw(WIDTH) << right << dades[i].poblacio << setw(WIDTH) << right << setprecision(USD_PRECISION) << fixed << dades[i].preu[0] << setw(WIDTH) << right << dades[i].preu[1] << setw(WIDTH) << right << dades[i].preu[2] << setw(WIDTH) << right << dades[i].preu[3] << setw(WIDTH) << right << dades[i].preu[4] << setw(WIDTH) << right << setprecision(RATE_PRECISION) << fixed << rate << endl;
	}

}
void reportB(Estat dades[]) {
	/*
	Pre:-
	Post: Escriu el report B per pantalla
	*/
	ordenar(dades, US_STATES, 'B');
	double rate = 0;
	for (int i = 0; i < US_STATES; i++)
	{
		if (dades[i].unitats[0] != 0) {
			rate = (double(dades[i].poblacio) / dades[i].unitats[0]);
		}
		else rate = 0;
		cout << setw(WIDTH_STATE_NAME) << left << dades[i].nom << setw(WIDTH) << right << dades[i].poblacio << setw(WIDTH) << right << dades[i].unitats[0] << setw(WIDTH) << right << dades[i].unitats[1] << setw(WIDTH) << right << dades[i].unitats[2] << setw(WIDTH) << right << dades[i].unitats[3] << setw(WIDTH) << right << dades[i].unitats[4] << setw(WIDTH) << right << setprecision(RATE_PRECISION) << fixed << rate << endl;

	}

}

void reportC(Estat dades[]) {
	/*
	Pre:-
	Post: Escriu el report C per pantalla
	*/
	ordenar(dades, US_STATES, 'C');
	double rate = 0;
	for (int i = 0; i < US_STATES; i++)
	{
		if (dades[i].enviaments[0] != 0) {
			rate = (double(dades[i].poblacio) / dades[i].enviaments[0]);
		}
		else rate = 0;
		cout << setw(WIDTH_STATE_NAME) << left << dades[i].nom << setw(WIDTH) << right << dades[i].poblacio << setw(WIDTH) << right << dades[i].enviaments[0] << setw(WIDTH) << right << dades[i].enviaments[1] << setw(WIDTH) << right << dades[i].enviaments[2] << setw(WIDTH) << right << dades[i].enviaments[3] << setw(WIDTH) << right << dades[i].enviaments[4] << setw(WIDTH) << right << setprecision(RATE_PRECISION) << fixed << rate << endl;

	}
}



int main()
{
	cout << "File" << US_CENSUS_2014 << " loaded [ " << US_STATES << " records ]" << endl << endl;
	cout << "=====[ Report S: Census states <Begin> ]=====" << endl << endl;
	ifstream cens(US_CENSUS_2014.c_str());
	Estat dades[US_STATES];
	if (!cens.is_open()) {
		cout << "ERROOOR" << endl;
		cin.get();
		return 1;
	}
	llegirCens(cens, dades);
	escruiureCens(dades);
	cout << "USER INPUT: File name of shipments?" << endl;
	string nom_fitxer;
	cin >> nom_fitxer;
	ifstream shipments(nom_fitxer.c_str());
	if (!shipments.is_open()) {
		cout << "ERROOOR" << endl;
		cin.get();
		return 1;
	}
	string aux;
	int data, k, temporal, omitits = 0, c = 0, trimestre;
	double diners;
	Estat estatsUnits;
	inicialitzar(dades, estatsUnits);
	shipments >> data;
	while (!shipments.eof()) {
		shipments >> aux;
		data = data % 10000;
		k = cercar(dades, aux);
		if (k == -1) {
			cout << "ERROR: Invalid state code: " << aux << " [ shipment " << c + 1 << " omitted ]." << endl;
			shipments >> temporal;
			shipments >> diners;
			omitits++;
		}
		else {
			trimestre = detTrimestre(data);
			llegirUnitatsIPreus(shipments, dades, estatsUnits, k, trimestre);
		}

		c++;
		shipments >> data;
	}
	cout << "File " << nom_fitxer << " loaded [ " << (c) << " records loaded with " << omitits << " records omitted by error = " << (c)-omitits << " ]" << endl << endl;
	sumTotals(dades, estatsUnits);
	cout << "== == = [Report 2014 Video - Games Springfield <Begin>] == == =" << endl << endl << "=====[ Report A: Quarter values ]=====" << endl << endl;
	cout << setw(WIDTH_STATE_NAME) << left << "State" << setw(WIDTH) << right << "Population" << setw(WIDTH) << right << "Anual" << setw(WIDTH) << right << "1Q" << setw(WIDTH) << right << "2Q" << setw(WIDTH) << right << "3Q" << setw(WIDTH) << right << "4Q" << setw(WIDTH) << right << "Population/Value" << endl;


	// report A
	double rate = 0;
	if (estatsUnits.preu[0] != 0) {
		rate = estatsUnits.poblacio / estatsUnits.preu[0];
	}
	cout << setw(WIDTH_STATE_NAME) << left << estatsUnits.nom << setw(WIDTH) << right << setprecision(USD_PRECISION) << fixed << estatsUnits.poblacio << setw(WIDTH) << right << estatsUnits.preu[0] << setw(WIDTH) << right << estatsUnits.preu[1] << setw(WIDTH) << right << estatsUnits.preu[2] << setw(WIDTH) << right << estatsUnits.preu[3] << setw(WIDTH) << right << estatsUnits.preu[4] << setw(WIDTH) << right << setprecision(RATE_PRECISION) << fixed << rate << endl;
	reportA(dades);

	//Report B
	if (estatsUnits.unitats[0] != 0) {
		rate = double(estatsUnits.poblacio) / estatsUnits.unitats[0];
	}
	else rate = 0;

	cout << endl << "=====[ Report B: Quarter units ]=====" << endl << endl;
	cout << setw(WIDTH_STATE_NAME) << left << "State" << setw(WIDTH) << right << "Population" << setw(WIDTH) << right << "Anual" << setw(WIDTH) << right << "1Q" << setw(WIDTH) << right << "2Q" << setw(WIDTH) << right << "3Q" << setw(WIDTH) << right << "4Q" << setw(WIDTH) << right << "Population/Units" << endl;
	cout << setw(WIDTH_STATE_NAME) << left << estatsUnits.nom << setw(WIDTH) << right << estatsUnits.poblacio << setw(WIDTH) << right << estatsUnits.unitats[0] << setw(WIDTH) << right << estatsUnits.unitats[1] << setw(WIDTH) << right << estatsUnits.unitats[2] << setw(WIDTH) << right << estatsUnits.unitats[3] << setw(WIDTH) << right << estatsUnits.unitats[4] << setw(WIDTH) << right << setprecision(RATE_PRECISION) << fixed << rate << endl;
	reportB(dades);
	cout << endl << "=====[ Report C: Quarter shipments ]=====" << endl << endl;
	//Report C

	if (estatsUnits.enviaments[0] != 0){
		rate = (double(estatsUnits.poblacio) / estatsUnits.enviaments[0]);
	}
	else rate = 0;
	cout << setw(WIDTH_STATE_NAME) << left << "State" << setw(WIDTH) << right << "Population" << setw(WIDTH) << right << "Anual" << setw(WIDTH) << right << "1Q" << setw(WIDTH) << right << "2Q" << setw(WIDTH) << right << "3Q" << setw(WIDTH) << right << "4Q" << setw(WIDTH) << right << "Population/Ships" << endl;
	cout << setw(WIDTH_STATE_NAME) << left << estatsUnits.nom << setw(WIDTH) << right << estatsUnits.poblacio << setw(WIDTH) << right << estatsUnits.enviaments[0] << setw(WIDTH) << right << estatsUnits.enviaments[1] << setw(WIDTH) << right << estatsUnits.enviaments[2] << setw(WIDTH) << right << estatsUnits.enviaments[3] << setw(WIDTH) << right << estatsUnits.enviaments[4] << setw(WIDTH) << right << setprecision(RATE_PRECISION) << fixed << rate << endl;
	reportC(dades);

	/*
	Aqui acaba la primera fase la practica mtijana
	*/

	cout << endl << "=====[ Report 2014 Video-Games Springfield <End> ]=====" << endl;



	cin.get(); //TREULO *******
	return 0;

}
