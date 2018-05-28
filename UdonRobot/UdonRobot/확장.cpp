#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <queue>
#include <functional>
#include <iostream>
using namespace std;

#define norm_rand() (rand()/(RAND_MAX + 1.0))

double expntl(double);

#define ARRIVAL_TIME 200.0
#define SERVICE_TIME 100.0
#define SIMULATION_TIME 200000.0
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

			// ���� �մ��� 1���̸�
			if (n == 0)
			{
				// tb = �κ� ��� ������ �ð�
				tb = elapsed_time;
				// ���� �մ� ������ �ð��� ���� �ð� + ���� ��� ������ �ð�
				//next_departure_time = elapsed_time + expntl(Ts);
			}

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

			if (n > 1)
			{
				next_departure_time = elapsed_time + expntl(Ts);
			}
			else
			{
				next_departure_time = simulation_time;
				// B = �κ� ����� �ð� (������ �ð�)
				B += elapsed_time - tb;
			}
		}
	}

	double Lambda = 1 / Ta;
	double Mew = 1 / Ts;
	X = C / elapsed_time;
	U = B / elapsed_time;
	L = s / elapsed_time;
	W = L / X;
	double service_time_per_customer = B / C;
	double Lq = LqS / elapsed_time;
	double Ls = LsS / elapsed_time;
	double Wq = W - service_time_per_customer;

	printf("Ta = %lf, Lambda = %lf\n", Ta, Lambda);
	printf("Ts = %lf, Mew = %lf\n", Ts, Mew);
	printf("S = %d\n", servers);
	printf("elapsed_time = %lf\n", elapsed_time);
	puts("-----------------------------------------------------------------");
	printf("�� ������ �� ��, A = %.0lf\n", A);
	printf("�� ������ �� ��, C = %.0lf\n", C);
	printf("�������� ó����, ��(arrival_rate) = %lf, X(throughput) = %lf\n", Lambda, X);
	puts("-----------------------------------------------------------------");
	printf("�κ��� ���ϴ� �ð�, B = %.2lf\n", B);
	printf("utilization, �κ��� �̿�� U = %lf, %lf\n", U, X * service_time_per_customer);
	printf("�մ� �� ����� ������ �ð� �� = %lf, %lf\n", service_time_per_customer, U / X);
	puts("-----------------------------------------------------------------");
	printf("mean customer no. in store, ��� �� �� L = %lf, %lf, %lf, %lf\n", L, W * C / elapsed_time, X * W, Lq + Ls);
	printf("�ٿ� ���� ��ٸ��� ��� �� ��, Lq = %lf\n", Lq);
	printf("���� �ϴ� ��� �� ��, Ls = %lf\n", Ls);
	printf("���Կ� �ӹ��� ��� �ð� W = %lf, %lf\n", W, Wq + service_time_per_customer);
	printf("����ϴ� �ð� Wq = %lf\n", Wq);

	puts("-----------------------------------------------------------------");
}

// ���� ���� �Լ�.
double expntl(double x)
{
	/* 'expntl' returns a psuedo -random variate from a negative exponen tial distribution with mean x */

	return(-x*log(norm_rand()));
	//return x;
}