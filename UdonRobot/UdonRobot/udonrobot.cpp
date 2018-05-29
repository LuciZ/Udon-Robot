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

// ���� �ð�. ��� 200 ���� �մ��� ����.
#define ARRIVAL_TIME 12.5
// ���� �ð�. ��� 100�� �Ŀ� �մ��� ����.
#define SERVICE_TIME 100.0
// ��ü �ùķ��̼� �ð�.
#define SIMULATION_TIME 200000.0
// �¼��� ����.
#define NUMBER_OF_SEATS 10

int main()
{
	// ť ����
	// ���Կ� ���� �� ������ �ð� ������ ���� �� ���� �̹Ƿ� ���� �ð��� �������� �켱���� ť�� ���.
	priority_queue<double, vector<double>, greater<double> > arrivalQueue;
	priority_queue<double, vector<double>, greater<double> > serviceQueue;

	// n: ���� ���Կ� �ִ� �մ� ��
	int n;
	// Ta: ��� Ta �� ���� �մ��� ����.
	// Ts: ��� Ts �� �Ŀ� �մ��� ����.
	double Ta = ARRIVAL_TIME, Ts = SERVICE_TIME, simulation_time = SIMULATION_TIME, next_arrival_time, next_departure_time,
		elapsed_time;

	// arrival_rate: ������, ��, 1�ʿ� ������ Ȯ��
	// service_rate: ������,��, 1�ʿ� ���� Ȯ��
	double arrival_rate = 1 / ARRIVAL_TIME, service_rate = 1 / SERVICE_TIME;
	// servers: �¼��� ����
	int numberOfSeats = NUMBER_OF_SEATS;

	// B: �¼��� ����� �ð�
	// C: ���� ����� �� ��
	// L: ���Կ� �ִ� ��� �� ��
	// S: �׷����� ���� (�ð�, �մԼ� �׷���), �ð����� ������ ���� ��� ���� ��.
	// tn: ������� ���� �̺�Ʈ ���� �ð� (����, ����)
	// U: utilization. �ý��� ����, �¼��� ����
	// W: ���� ��� �ӹ��� �ִ� �ð�
	// X: throughput. �ð��� �� ���� �� ��
	double B, C, L, s, tn, U, W, X;

	// Lq (��� ��� �� ��), Ls (��� ���� ���� �� ��)�� ���ϱ� ���� ����.
	double  LqS = 0.0, LsS = 0.0;

	// ������ �� ��.
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
			// ���� ���Կ� �ִ� �մ� ���� ��� ���� �մ� �� + ���� ���� �մ� ��.
			n = arrivalQueue.size() + serviceQueue.size();
			
			// ����� �ð� = ���� �ð�
			elapsed_time = next_arrival_time;

			// s, LqS, LsS = �׷����� ���� (�ð�, �մԼ� �׷���)
			// n = ���� �ִ� ���� ��
			// elapsed_time - tn = ������� ���� �̺�Ʈ ���� �ð�
			s += n * (elapsed_time - tn);			
			LqS += arrivalQueue.size() * (elapsed_time - tn);
			LsS += serviceQueue.size() * (elapsed_time - tn);

			// �ð� ������Ʈ.
			tn = elapsed_time;

			// �մ� �� �� ����.
			arrivalQueue.push(elapsed_time);
			A++;

			// ���� ���� �ð��� ����� �ð� + ���� ��� ���� �ð�
			next_arrival_time = elapsed_time + expntl(Ta);

			// ���Կ� �� �¼��� ������
			if (serviceQueue.size() < numberOfSeats)
			{
				// �ٷ� ���� ����
				serviceQueue.push(elapsed_time + expntl(Ts));
				arrivalQueue.pop();
			}
		}
		// ���� ���� �ް� ����.
		else
		{
			/* event 2 : customer departure */
			// ���� ���Կ� �ִ� �մ� ���� ��� ���� �մ� �� + ���� ���� �մ� ��.
			n = arrivalQueue.size() + serviceQueue.size();

			// ����� �ð� = ���� ������ �ð�
			elapsed_time = serviceQueue.top();

			// s, LqS, LsS = �׷����� ���� (�ð�, �մԼ� �׷���)
			// n = ���� �ִ� ���� ��
			// elapsed_time - tn = ������� ���� �̺�Ʈ ���� �ð�
			s += n * (elapsed_time - tn);
			LqS += arrivalQueue.size() * (elapsed_time - tn);
			LsS += (serviceQueue.size()) * (elapsed_time - tn);


			// �ð� ������Ʈ.
			tn = elapsed_time;

			// �� �� ����.
			serviceQueue.pop();
			C++;

			// �� �¼��� ����� ����ϴ� �մ� ���� ����.
			if (arrivalQueue.empty() == false && serviceQueue.size() < numberOfSeats)
			{
				// �ٷ� ���� ����
				serviceQueue.push(elapsed_time + expntl(Ts));
				arrivalQueue.pop();
			}
		}
	}

	// �¼��� ����� �ð��� ���� ���񽺵� �ð�.
	B = LsS;
	// �̷л��� ���ٿ� �� ��.
	double real_Lambda = 1 / Ta;
	double real_Mew = 1 / Ts;
	// ���� �ùķ��̼��� ����, ��(X) ��.
	double Lambda = A / elapsed_time;
	X = C / elapsed_time;
	// �¼��� �̿���� ��ü ����� �ð��� �¼� ���� ���� ��.
	U = B / elapsed_time / numberOfSeats;
	// ���Կ� �ִ� ��� �� ��, ��� �� ��, ���� �� ���� �׷����� ���̸� �ð����� ���� ��.
	L = s / elapsed_time;
	double Lq = LqS / elapsed_time;
	double Ls = LsS / elapsed_time;
	// ���� �ӹ��� �ð��� ������ ��� �� ���� ���� �ð� �� �� ���� ���� ��.
	W = L / X;
	// �� �� ���� �ð��� �¼��� ��� �ð��� ���� �� ���� ���� ��.
	double service_time_per_customer = B / C;
	// ���� ����ϴ� �ð��� ����ϴ� ��� �� ���� �ð��� ���� �� ���� ���� ��.
	double Wq = Lq / Lambda;

	// ���.
	puts("�κ��쵿���� �ùķ��̼�. By 2013111995 ver2.0");
	puts("-----------------------------------------------------------------");
	printf("Ta = %f, Lambda = %f\n", Ta, real_Lambda);
	printf("Ts = %f, Mew = %f\n", Ts, real_Mew);
	printf("S = %d\n", numberOfSeats);
	printf("elapsed_time = %f\n", elapsed_time);
	puts("-----------------------------------------------------------------");
	printf("�� ������ �� ��, A = %.0lf\n", A);
	printf("�� ������ �� ��, C = %.0lf\n", C);
	printf("�������� ó����, ��(arrival_rate) = %f, X(throughput) = %f\n", Lambda, X);
	puts("-----------------------------------------------------------------");
	printf("�¼��� �����Ǵ� �ð�, B = %f, %f\n", B, LsS);
	printf("utilization, �¼��� �̿�� U = %f, %f\n", U, X * service_time_per_customer / numberOfSeats);
	printf("�մ� �� ����� ������ �ð� �� = %f, %f\n", service_time_per_customer, U / X * numberOfSeats);
	puts("-----------------------------------------------------------------");
	printf("mean customer no. in store, ��� �� �� L = %f, %f, %f, %f\n", L, W * C / elapsed_time, X * W, Lq + Ls);
	printf("�ٿ� ���� ��ٸ��� ��� �� ��, Lq = %f\n", Lq);
	printf("���� �ϴ� ��� �� ��, Ls = %f\n", Ls);
	printf("���Կ� �ӹ��� ��� �ð� W = %f\n", W);
	printf("����ϴ� �ð� Wq = %f\n", Wq);
	puts("-----------------------------------------------------------------");
}

// ���� ���� �Լ�.
double expntl(double x)
{
	/* 'expntl' returns a psuedo -random variate from a negative exponen tial distribution with mean x */
	return(-x*log(norm_rand()));
}