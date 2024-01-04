// David Taylor, Georgia Tech, 7/15/2023

#include "F2837xD_device.h"
#include "Quanser_servo_framework.h"
#include "math.h"

void updateController(void);
void logData(void);
void sendData(void);
void resetExperiment(void);
void assignParameters(void);

#define N 5000
#define T 0.001

// ----------------------------------------------
Uint32 new_mode = 0;
Uint32 old_mode = 0;

Uint32 time_index;
Uint32 log_index;

Uint32 com_status = 0;

float32 t;
float32 r, log_r[N];
float32 u, log_u[N];
float32 y1, log_y1[N];
float32 y2, log_y2[N];

float32 x_e1, x_e2, x_r;                                            // Estimator & Regulator State variables
float32 u_c;
float32  y_dot_hat1, y_dot_hat2;                                    // Output
float32 a, b, c, d;                                                 // Estimator parameters
float32 k1, k2, k3, k4, k5;                                         // Regulator parameters
float32 lamda_c, lamda_r;                                           // Regulator & Estimator Bandwidth
float32 kappa;                                                      //
float32 U_max = 6;                                                  // Actuator Saturation Limit (V)
// -----------------------------------------------
void main (void)
{
    initializeMicrocontroller(T);
    initializePlant();
    assignParameters();
    resetExperiment();
    CpuTimer0Regs.TCR.bit.TSS = 0;

    while(1)
    {
        feedWD();
    }
}
// +----------------+
// |    isrSCI      |
// + ---------------+
interrupt void isrSCI (void)
{
    new_mode = SciaRegs.SCIRXBUF.bit.SAR;
    PieCtrlRegs.PIEACK.all = M_INT9;
}

interrupt void isrTMR (void)
{
    exchangePacketSPI();
    com_status = getCommunicationStatus();

    updateController();
    PieCtrlRegs.PIEACK.all = M_INT1;
}
// ------------------------------------------------------------
void updateController (void)
{
    // update sensor measurements
    y1 = getMotorPosition();
    y2 = getPendulumPosition()-M_PI;

    // when controller is active
    if (fabsf(y2) < 0.1745)
    {
        // end trial (let pendulum fall)
        if ((new_mode == 0) && (old_mode == 1))
        {
            CpuTimer0Regs.TCR.bit.TSS = 1;
            sendData();
            old_mode = 0;
            CpuTimer0Regs.TCR.bit.TSS = 0;
        }

        // update reference
        if (new_mode == 1)
        {
            // assign green color
            setPlantLED(0,255,0);

            t = time_index*T;

            // step reference case
            // --------------------
            if (t >= 0 && t < 7.5) {
                r = 2;
            } else if(t >= 7.5 && t < 15) {
                r = 0;
            } else if(t >= 15 && t < 22.5) {
                r = 2;
            } else if(t >= 22.5 && t < 30) {
                r = 0;
            }

            // log data conditionally
            if (time_index%6 == 0)
            {
                logData();
            }

            time_index++;
            old_mode = 1;
        }
        else
        {
            // assign red color
            setPlantLED(255,0,0);

            // zero reference case
            r = 0;

            old_mode = 0;
        }
        // +--------------------+
        // |  update estimator  |
        // +--------------------+
        y_dot_hat1 = c*x_e1 + d*y1;
        x_e1 += T*(a*x_e1 + b*y1);

        y_dot_hat2 = c*x_e2 + d*y2;
        x_e2 += T*(a*x_e2 + b*y2);
        // +------------------+
        // | update regulator |
        // +------------------+

        u_c = -k1*y1 - k2*y2 - k3*y_dot_hat1 - k4*y_dot_hat2 - k5*x_r;                // uc[k] = − k1y1[k] − k2y2[k] − k3 ^.y1[k] − k4 ^.y2[k] − k5xr[k] (4)
        u = u_c;
        // Anit-Windup Compensation                     // (5) u[k] = sat(u_c[k])
        // -------------------------
        if (u_c > U_max) {
            u = U_max;
        } else if (u_c < -U_max) {
            u = -U_max;
        }

        x_r += T*(y1 - r + kappa*(u_c - u));

        // update actuator command
        setMotorVoltage(u);
    }

    // when controller is not active
    else
    {
        resetExperiment();
    }
}

void logData(void)
{
    if (log_index < N)
    {
        log_r[log_index] = r;
        log_u[log_index] = u;
        log_y1[log_index] = y1;
        log_y2[log_index] = y2;
        log_index++;
    }
}

void sendData(void)
{
    Uint32 send_index;
    for (send_index = 0; send_index < N; send_index++)
    {
        feedWD();
        transmitFloatSCI(log_r[send_index]);
        transmitFloatSCI(log_u[send_index]);
        transmitFloatSCI(log_y1[send_index]);
        transmitFloatSCI(log_y2[send_index]);
    }
}

void resetExperiment(void)
{
    // assign blue color
    setPlantLED(0,0,255);

    // initialize actuator command
    setMotorVoltage(0);

    // initialize index variables
    time_index = 0;
    log_index = 0;

    // initialize logic variables
    new_mode = 0;
    old_mode = 0;

    // initialize state variables
    x_e1 = 0;
    x_e2 = 0;
    x_r = 0;
}

void assignParameters(void)
{
    // assign estimator parameters
    // ---------------------------
    lamda_r = 8;                            // Regulator Bandwidth (rad/s)
    lamda_c = 50;                           // Estimator Bandwidth (rad/s)

    a = -lamda_c;
    b = -pow(lamda_c, 2);
    c = 1;
    d = lamda_c;
    // assign regulator parameters
    // ---------------------------
    k1 = -4.6899;                                 // <----------------------- Edit here
    k2 = 27.459;
    k3 = -1.4785;
    k4 = 2.2304;
    k5 = -7.4948;
    kappa = lamda_r/k5;

}
