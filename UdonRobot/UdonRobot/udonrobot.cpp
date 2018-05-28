#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <queue>
#include <functional>
#include <iostream>
#include <fstream>
using namespace std;

#define norm_rand() (rand()/(RAND_MAX + 1.0))

double expntl(double);

#define ARRIVAL_TIME 2.0
#define SERVICE_TIME 10.0
#define SIMULATION_TIME 20000.0
#define SERVERS 10

int main()
{
	// ť ����
	priority_queue<double, vector<double>, greater<double> > arrivalQueue;
	priority_queue<double, vector<double>, greater<double> > serviceQueue;

	// n: ���� ���Կ� �ִ� �մ� ��
	int n;
	// Ta: ��� Ta �� ���� �մ��� ����.
	// Ts: ��� Ts �� �Ŀ� �մ��� ����.
	double Ta = ARRIVAL_TIME, Ts = SERVICE_TIME, simulation_time = SIMULATION_TIME, next_arrival_time, next_departure_time,
		elapsed_time;

	// arrival_rate: ������, ��, 1 �ð��� ������ Ȯ��
	// service_rate: ������,��, 1 �ð��� ���� Ȯ��
	double arrival_rate = 1 / ARRIVAL_TIME, service_rate = 1 / SERVICE_TIME;
	// servers: ���� ����, �¼��� ����
	int servers = SERVERS;

	// B: �κ� ����� �ð�
	// C: ���� ����� �� ��
	// L: �ý��ۿ� �ִ� ��� �� ��
	// S: �׷����� ���� (�ð�, �մԼ� �׷���), �ð����� ������ �ý��� ��� ���� ��.
	// tb: �κ� ��� ������ �ð�
	// tn: ������� ���� �̺�Ʈ ���� �ð� (����, ����)
	// U: utilization. �ý��� ����, �κ��� ����
	// W: ���� ��� �ӹ��� �ִ� �ð�
	// X: throughput. �ð��� �� ���� �� ��
	double B, C, L, s, tb, tn, U, W, X;

	// Lq, Ls�� ���ϱ� ���� ����.
	double  LqS = 0.0, LsS = 0.0;

	double A = 0;

	n = 0; next_arrival_time = 0.0; next_departure_time = simulation_time; elapsed_time = 0.0;
	B = s = 0.0; C = 0; tn = elapsed_time;

	/* Seed the random-number generator with current time so that the number will be dif ferent every time we run.*/
	srand((unsigned)time(NULL));

	while (elapsed_time < simulation_time)
	{
		// �մ��� ����.
		if (serviceQueue.empty() || next_arrival_time < serviceQueue.top())
		{
			/* event 1 : customer arrival */

			n = arrivalQueue.size() + serviceQueue.size();


			// ����� �ð� = ���� �ð�
			elapsed_time = next_arrival_time;

			// s = �׷����� ���� (�ð�, �մԼ� �׷���)
			// n = ���� �ִ� ���� ��
			// elapsed_time - tn = ������� ���� �̺�Ʈ ���� �ð�
			s += n*(elapsed_time - tn);
			
			LqS += arrivalQueue.size() * (elapsed_time - tn);
			LsS += serviceQueue.size() * (elapsed_time - tn);

			tn = elapsed_time;
			A++;

			arrivalQueue.push(elapsed_time);


			// ���� ���� �ð��� ����� �ð� + ���� ��� ���� �ð�
			next_arrival_time = elapsed_time + expntl(Ta);


			if (serviceQueue.size() < servers)
			{
				// �ٷ� ���� ����
				serviceQueue.push(elapsed_time + expntl(Ts));
				arrivalQueue.pop();
			}
		}
		else
		{
			/* event 2 : customer departure */

			n = arrivalQueue.size() + serviceQueue.size();

			//elapsed_time = next_departure_time;
			elapsed_time = serviceQueue.top();
			serviceQueue.pop();

			s += n * (elapsed_time - tn);

			LqS += arrivalQueue.size() * (elapsed_time - tn);
			LsS += (serviceQueue.size() + 1) * (elapsed_time - tn);

			tn = elapsed_time;
			C++;


			if (arrivalQueue.empty() == false && serviceQueue.size() < servers)
			{
				// �ٷ� ���� ����
				serviceQueue.push(elapsed_time + expntl(Ts));
				arrivalQueue.pop();
			}
		}
	}

	B = LsS;
	double real_Lambda = 1 / Ta;
	double real_Mew = 1 / Ts;
	double Lambda = A / elapsed_time;
	X = C / elapsed_time;
	//U = B / elapsed_time;
	U = LsS / elapsed_time/ servers;
	L = s / elapsed_time;
	W = L / X;
	double service_time_per_customer = B / C;
	double Lq = LqS / elapsed_time;
	double Ls = LsS / elapsed_time;
	double Wq = Lq / Lambda;

	printf("Ta = %f, Lambda = %f\n", Ta, real_Lambda);
	printf("Ts = %f, Mew = %f\n", Ts, real_Mew);
	printf("S = %d\n", servers);
	printf("elapsed_time = %f\n", elapsed_time);
	puts("-----------------------------------------------------------------");
	printf("�� ������ �� ��, A = %.0lf\n", A);
	printf("�� ������ �� ��, C = %.0lf\n", C);
	printf("�������� ó����, ��(arrival_rate) = %f, X(throughput) = %f\n", Lambda, X);
	puts("-----------------------------------------------------------------");
	printf("�κ��� ���ϴ� �ð�, B = %f, %f\n", B, LsS);
	printf("utilization, �κ��� �̿�� U = %f, %f\n", U, X * service_time_per_customer / servers);
	printf("�մ� �� ����� ������ �ð� �� = %f, %f\n", service_time_per_customer, U / X * servers);
	puts("-----------------------------------------------------------------");
	printf("mean customer no. in store, ��� �� �� L = %f, %f, %f, %f\n", L, W * C / elapsed_time, X * W, Lq + Ls);
	printf("�ٿ� ���� ��ٸ��� ��� �� ��, Lq = %f\n", Lq);
	printf("���� �ϴ� ��� �� ��, Ls = %f\n", Ls);
	printf("���Կ� �ӹ��� ��� �ð� W = %f, %f\n", W, Wq + 1 / X);
	printf("����ϴ� �ð� Wq = %f\n", Wq);

	puts("-----------------------------------------------------------------");
}

// ���� ���� �Լ�.
double expntl(double x)
{
	/* 'expntl' returns a psuedo -random variate from a negative exponen tial distribution with mean x */

	return(-x*log(norm_rand()));
	//return x;
}