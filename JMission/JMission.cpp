// JMission.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "JMission.h"
#include <Eigen/Dense>
#include <string>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <vector>
# define M_PI           3.14159265358979323846
//using namespace Eigen;

#define MAX_LOADSTRING 100

double normalize(double value, double min, double max) {
    return 2 * (value - min) / (max - min) - 1;
}
double denormalize(double value, double min, double max) {
    return (value + 1) * (max - min) / 2 + min;
}
Eigen::Matrix<double, 1, 3> normalizeVec3(Eigen::Matrix<double, 1, 3> value, double min, double max) {

    for (size_t i = 0; i < 3; i++)
    {
        value(i) = normalize(value(i), min, max);
    }

    return value;
}
Eigen::Matrix<double, 1, 3> denormalizeVec3(Eigen::Matrix<double, 1, 3> value, double min, double max) {

    for (size_t i = 0; i < 3; i++)
    {
        value(i) = denormalize(value(i), min, max);
    }
    return value;
}



// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);







//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_JMISSION));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_JMISSION);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, 600, 600, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//

CHAR getChar(WPARAM wparam)
{
    CHAR a = NULL;
    for (char ch = 'a'; ch <= 'z'; ch++)
    {
        if (wparam == ch)
        {
            a = ch;
        }
    }
    for (char ch = 'A'; ch <= 'Z'; ch++)
    {
        if (wparam == ch)
        {
            a = ch;
        }
    }
    return a;
}

static Eigen::Matrix<double, 4, 4> BaseMatrix;
static Eigen::Matrix<double, 4, 4> RotationMartix;
static Eigen::Matrix<double, 4, 4> ScaleMatrix;
static Eigen::Matrix<double, 4, 4> ReflectionMatrix;
static Eigen::Matrix<double, 4, 4> TranslationMatrix;
static Eigen::Matrix<double, 1, 4> vec1;
static Eigen::Matrix<double, 1, 4> vec2;
static Eigen::Matrix<double, 1, 4> vec3;
static Eigen::Matrix<double, 4, 4> DeViewPortMatrix3;
static Eigen::Matrix<double, 4, 4> SuperMatrix;
static Eigen::Matrix<double, 4, 4> TriangleWorldMatrix;
static Eigen::Matrix<double, 1, 4> vecBLR;
static Eigen::Matrix<double, 1, 4> vecBLRA;
static Eigen::Matrix<double, 1, 4> vecBLG;
static Eigen::Matrix<double, 1, 4> vecBLGA;
static Eigen::Matrix<double, 1, 4> vecBLB;
static Eigen::Matrix<double, 1, 4> vecBLBA;
static Eigen::Matrix<double, 4, 4> PerspectiveProjectionMatrix;
static Eigen::Matrix<double, 4, 4> CameraViewMatrix;
static Eigen::Matrix<double, 4, 4> VeiwProjViewportMatrix;
static Eigen::Matrix<double, 4, 4> CameraYawMatrix;
static Eigen::Matrix<double, 4, 4> CameraPitchMatrix;
static Eigen::Matrix<double, 4, 4> CameraRollMatrix;
static Eigen::Matrix<double, 4, 4> CameraTranslateMatrix;
static std::vector<Eigen::Matrix<double, 1, 4>> vecsL1;
static std::vector<Eigen::Matrix<double, 1, 4>> vecsL1A;
static std::vector<Eigen::Matrix<double, 1, 4>> vecsL2;
static std::vector<Eigen::Matrix<double, 1, 4>> vecsL2A;
static std::vector<Eigen::Matrix<double, 1, 4>> vecsL3;
static std::vector<Eigen::Matrix<double, 1, 4>> vecsL3A;
static Eigen::Matrix<double, 1, 4> v;
static Eigen::Matrix<double, 1, 4> v1;

static bool triggerRotate = true;
static bool triggerReflect = true;
static bool triggerTrans = true;
static int triggerDilation = 1;
static bool hi = true;
static bool hiForRotation = false;
static int frameNum = 0;
static double walk = 0;
static double radians = 0;
static double radYaw = 0;
static double radPitch = 0;
static double radRoll = 0;
static double Yaw = 0;
static double Pitch = 0;
static double Roll = 0;
static double w = 600.0;
static double random_number;
//static double ahead = 0;
//static double sideStep = 0; 
//static double jump = 0;
static Eigen::Matrix<double, 1, 4> CameraPosition;
static double fov = 45 * (M_PI / 360);
static double aspect = 0;
static int a, b, c, d;


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    RECT clientRect;
    
    switch (message)
    {
    case WM_CREATE:
        SetTimer(hWnd, 1, 100, NULL);
        SetWindowPos(hWnd, HWND_TOP, 0, 0, 600, 600, SWP_NOMOVE);
        break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_CONTROL:
            MessageBox(NULL, TEXT("Control Key Down"), TEXT("Key Down"), MB_OK);
            break;
        }
        if (GetAsyncKeyState('W') & (1 << 16))
        {
            /*jump += 0.01;*/
            Eigen::Matrix<double, 1, 4>  Dir;
            Dir << 0, 0, 0.01, 0;
            Dir *= CameraViewMatrix;
            CameraPosition += Dir;
            //if (jump > 1) { jump = -1; }
            break;
        }
        if (GetAsyncKeyState('A') & (1 << 16))
        {
            /*sideStep -= 0.01;*/
            //if (sideStep > 1) { sideStep = -1; }
            Eigen::Matrix<double, 1, 4>  Dir;
            Dir << -0.01, 0, 0, 0;
            Dir *= CameraViewMatrix;
            CameraPosition += Dir;
            break;
        }
        if (GetAsyncKeyState('S') & (1 << 16))
        {
            /*jump -= 0.01;*/
            //if (jump < -1) { jump = 1; }
            Eigen::Matrix<double, 1, 4>  Dir;
            Dir << 0, 0, -0.01, 0;
            Dir *= CameraViewMatrix;
            CameraPosition += Dir;
            break;
        }
        if (GetAsyncKeyState('D') & (1 << 16))
        {
           /* sideStep += 0.01;*/
            //if (sideStep < -1) { sideStep = 1; }
            Eigen::Matrix<double, 1, 4>  Dir;
            Dir << 0.01, 0, 0, 0;
            Dir *= CameraViewMatrix;
            CameraPosition += Dir;
            break;
        }
        if (GetAsyncKeyState('Z') & (1 << 16))
        {
            /*ahead += 0.01;*/
            //if (ahead > 1) { ahead = -1; }
            Eigen::Matrix<double, 1, 4>  Dir;
            Dir << 0, 0.01, 0, 0;
            Dir *= CameraViewMatrix;
            CameraPosition += Dir;
            break;
        }
        if (GetAsyncKeyState('X') & (1 << 16))
        {
            /*ahead -= 0.01;*/
            //if (ahead < -1) { ahead = 1; }
            Eigen::Matrix<double, 1, 4>  Dir;
            Dir << 0, -0.01, 0, 0;
            Dir *= CameraViewMatrix;
            CameraPosition += Dir;
            break;
        }
        if (GetAsyncKeyState(VK_RIGHT) & (1 << 16))
        {
            Yaw += 2 * (M_PI / 180.0);
            break;
        }
        if (GetAsyncKeyState(VK_LEFT) & (1 << 16))
        {
            Yaw -= 2 * (M_PI / 180.0);
            break;
        }
        if (GetAsyncKeyState(VK_UP) & (1 << 16))
        {
            Pitch += 2 * (M_PI / 180.0);
            break;
        }
        if (GetAsyncKeyState(VK_DOWN) & (1 << 16))
        {
            Pitch -= 2 * (M_PI / 180.0);
            break;
        }
        if (GetAsyncKeyState('C') & (1 << 16))
        {
            Roll += 2 * (M_PI / 180.0);
            break;
        }
        if (GetAsyncKeyState('V') & (1 << 16))
        {
            Roll -= 2 * (M_PI / 180.0);
            break;
        }

        break;
    case WM_KEYUP:

            /*if (!(GetAsyncKeyState('D') & 0x8000))
            {
                
                break;
            }*/

            
            switch (wParam)
            {
            case VK_SHIFT:
                MessageBox(NULL, TEXT("Shift Key Up"), TEXT("Key Up"), MB_OK);
                break;
            }
            break;
        
        break;
    case WM_CHAR:
        if (getChar(wParam) == 'q' || getChar(wParam) == 'Q')
        {
            if (triggerRotate) {
                triggerRotate = false;
                hiForRotation = true;
            }
            else {
                triggerRotate = true;
                hiForRotation = false;
                RotationMartix << cos(0), sin(0), 0, 0,
                    -sin(0), cos(0), 0, 0,
                    0, 0, 1, 0,
                    0, 0, 0, 1;
            }
            
            break;
        }

        if (getChar(wParam) == 't' || getChar(wParam) == 't')
        {
            if (triggerDilation == 1) {
                triggerDilation = 2;
                ScaleMatrix << 2, 0, 0, 0,
                    0, 2, 0, 0,
                    0, 0, 2, 0,
                    0, 0, 0, 1;
                break;
            }
            if (triggerDilation == 2) {
                triggerDilation = 3;
                ScaleMatrix << 3, 0, 0, 0,
                    0, 3, 0, 0,
                    0, 0, 3, 0,
                    0, 0, 0, 1;
                break;
            }
            else {
                triggerDilation = 1;
                ScaleMatrix << 1, 0, 0, 0,
                    0, 1, 0, 0,
                    0, 0, 1, 0,
                    0, 0, 0, 1;
            }
        }

        if (getChar(wParam) == 'e' || getChar(wParam) == 'E')
        {
            if (triggerReflect) {
                triggerReflect = false;
                ReflectionMatrix *= ReflectionMatrix;
            }
            else {
                triggerReflect = true;
                ReflectionMatrix << 1, 0, 0, 0,
                    0, -1, 0, 0,
                    0, 0, 1, 0,
                    0, 0, 0, 1;
            }
            
        }

        if (getChar(wParam) == 'r' || getChar(wParam) == 'R')
        {
            srand(time(NULL));
            random_number = rand() % (550 + 1);
            random_number = normalize(random_number, 0, 550);
            if (triggerTrans) {
                triggerTrans = false;
                TranslationMatrix << 1, 0, 0, 0,
                    0, 1, 0, 0,
                    0, 0, 1, 0,
                    random_number, random_number, 0, 1;
                
            }
            else {
                triggerTrans = true;
                TranslationMatrix << 1, 0, 0, 0,
                    0, 1, 0, 0,
                    0, 0, 1, 0,
                    0, 0, 0, 1;
            }
        }
        return 0;
        break;
    case WM_PAINT:
        {
            
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            HPEN hpen;
            GetClientRect(hWnd, &clientRect);
            int maxX = clientRect.right - clientRect.left;
            int maxY = clientRect.bottom - clientRect.top;
            int centerX = (clientRect.right - clientRect.left) / 2;
            int centerY = (clientRect.bottom - clientRect.top) / 2;

            aspect = maxX / maxY;
            HBRUSH newBrush = CreateSolidBrush(RGB(28, 28, 28));
            HGDIOBJ oldBrush = SelectObject(hdc, newBrush);

            SetTextColor(hdc, RGB(130, 129, 181));
            SetBkColor(hdc, RGB(28, 28, 28));

            DrawPaintArea(hdc, 0, 0, maxX, maxY);
            
            //for (double i = 0; i != 540; i += 90)
            //{
            //    MoveToEx(hdc, 0, i, NULL);
            //    LineTo(hdc, 600, i);
            //}

            //for (double i = 0; i != 584; i += 73)
            //{
            //    MoveToEx(hdc, i, 0, NULL);
            //    LineTo(hdc, i, 600);
            //}

            if (hi == true)
            {
                hi = false;

                CameraPosition << 0, 0, -2, 0;

                BaseMatrix << 1, 0, 0, 0,
                    0, 1, 0, 0,
                    0, 0, 1, 0,
                    0, 0, 0, 1;

                RotationMartix << cos(0), sin(0), 0, 0,
                    -sin(0), cos(0), 0, 0,
                    0, 0, 1, 0,
                    0, 0, 0, 1;

                ScaleMatrix << 0.5, 0, 0, 0,
                    0, 0.5, 0, 0,
                    0, 0, 0.5, 0,
                    0, 0, 0, 1;

                ReflectionMatrix << 1, 0, 0, 0,
                    0, -1, 0, 0,
                    0, 0, 1, 0,
                    0, 0, 0, 1;

                TranslationMatrix << 1, 0, 0, 0,
                    0, 1, 0, 0,
                    0, 0, 1, 0,
                    0, 0, 0, 1;

                DeViewPortMatrix3 << (double)maxX / 2, 0, 0, 0,
                    0, (double)-maxY / 2, 0, 0,
                    0, 0, 1, 0,
                    (double)maxX / 2, (double)maxY / 2, 0, 1;

                CameraViewMatrix << 1, 0, 0, 0,
                    0, 1, 0, 0,
                    0, 0, 1, 0,
                    0, 0, 0, 1;

                vecsL1.reserve(64);
                vecsL1A.reserve(64);
                vecsL2.reserve(64);
                vecsL2A.reserve(64);
                vecsL3.reserve(64);
                vecsL3A.reserve(64);


                for (double i = -1; i < 1; i += 0.25)
                {
                    for (double j = -1; j < 1; j += 0.25)
                    {
                        v << i, j, i, 1;
                        vecsL1.push_back(v);

                        v1 <<i, i, 1, 1;
                        vecsL1A.push_back(v1);
                    }
                }

                /*static double Far2 = 10000; static double Near2 = 0.1;
                PerspectiveProjectionMatrix << 1/(aspect*tan(fov/2)), 0, 0, 0,
                    0, 1/(tan(fov/2)), 0, 0,
                    0, 0, -(Far2+Near2)/(Far2-Near2), -1,
                    0, 0, -((2*Far2*Near2)/(Far2-Near2)), 1;*/
                fov = 120 * (M_PI / 180);
                double yScale = 1/tan(fov / 2);
                double xScale = yScale / aspect;
                double zf = 10000; double zn = 0.1;
                PerspectiveProjectionMatrix << xScale, 0, 0, 0,
                0, yScale, 0, 0,
                0, 0, zf / (zn - zf), zn* zf / (zn - zf),
                0, 0,       -1,     0;
            }



            if (hiForRotation)
            {
                RotationMartix << cos(radians), sin(radians), 0, 0,
                    -sin(radians), cos(radians), 0, 0,
                    0, 0, 1, 0,
                    0, 0, 0, 1;
            }

            Yaw = 0;
            Pitch = 0;// 45 * (M_PI / 180);
            Roll = 0;// -135 * (M_PI / 180);

            /*CameraViewMatrix << cos(Yaw) * cos(Pitch), (cos(Yaw)* sin(Pitch)* sin(Roll)) - (sin(Yaw) * cos(Roll)), (cos(Yaw)* sin(Pitch)* cos(Roll)) + (sin(Yaw) * sin(Roll)), 0,
                sin(Yaw)* cos(Pitch), (sin(Yaw)* sin(Pitch)* sin(Roll)) + (cos(Yaw) * cos(Roll)), (sin(Yaw)* sin(Pitch)* cos(Roll)) - (cos(Yaw) * sin(Roll)), 0,
                -sin(Pitch), cos(Pitch)* sin(Roll), cos(Pitch)* cos(Roll), 0,
                sideStep, jump, ahead, 1;*/

            CameraYawMatrix << cos(Yaw), -sin(Yaw), 0, 0,
                sin(Yaw), cos(Yaw), 0, 0,
                0, 0, 1, 0,
                0, 0, 0, 1;
            CameraPitchMatrix << cos(Pitch), 0, sin(Pitch), 0,
                0, 1, 0, 0,
                -sin(Pitch), 0, cos(Pitch), 0,
                0, 0, 0, 1;
            CameraRollMatrix << 1, 0, 0, 0,
                0, cos(Roll), -sin(Roll), 0,
                0, sin(Roll), cos(Roll), 0,
                0, 0, 0, 1;
            CameraTranslateMatrix << 1, 0, 0, 0,
                0, 1, 0, 0,
                0, 0, 1, 0,
                CameraPosition(0), CameraPosition(1), CameraPosition(2), 1;

            CameraViewMatrix = /*CameraRollMatrix  *  CameraYawMatrix * */ CameraTranslateMatrix;

            //radYaw = 45 * (M_PI / 180);
            radPitch = 0;//10 * (M_PI / 180);
            radRoll = 0;//45 * (M_PI / 180);

            TriangleWorldMatrix << cos(radYaw)*cos(radPitch), (cos(radYaw)*sin(radPitch)*sin(radRoll))-(sin(radYaw)*cos(radRoll)), (cos(radYaw)*sin(radPitch)*cos(radRoll))+(sin(radYaw)*sin(radRoll)), 0,
                                   sin(radYaw)*cos(radPitch), (sin(radYaw)*sin(radPitch)*sin(radRoll))+(cos(radYaw)*cos(radRoll)), (sin(radYaw)*sin(radPitch)*cos(radRoll))-(cos(radYaw)*sin(radRoll)), 0,
                                    -sin(radPitch), cos(radPitch)*sin(radRoll), cos(radPitch)*cos(radRoll), 0,
                                    0, 0, 0, 1;

            //vec1 << centerX, centerY + 50, 0;
            //vec2 << centerX + 50, centerY - 50, 0;
            //vec3 << centerX - 50, centerY - 50, 0;

            SuperMatrix = RotationMartix * ScaleMatrix * TranslationMatrix * TriangleWorldMatrix;
            VeiwProjViewportMatrix = /*SuperMatrix **/ CameraViewMatrix.inverse() * PerspectiveProjectionMatrix * DeViewPortMatrix3;

            hpen = CreatePen(PS_SOLID, 1, RGB(123, 0, 28));
            SelectObject(hdc, hpen);





            /*for (double i = 0; i != 540; i += 0.25)
            {
                Eigen::Matrix<double, 1, 4> vecsL1(i);
                MoveToEx(hdc, 0, i, NULL);
                LineTo(hdc, 600, i);
            }*/

            /*hpen = CreatePen(PS_SOLID, 1, RGB(0, 56, 43));
            SelectObject(hdc, hpen);

            for (double i = 0; i != 584; i += 73)
            {
                MoveToEx(hdc, i, 0, NULL);
                LineTo(hdc, i, 600);
            }

            hpen = CreatePen(PS_SOLID, 1, RGB(169, 29, 17));
            SelectObject(hdc, hpen);

            for (double i = 0, j = 0; i != 584; i += 73)
            {
                MoveToEx(hdc, i, 0, NULL);
                LineTo(hdc, i, 600);
            }*/
            
            //vec1 = normalizeVec3(vec1, 0, w);
            vec1 << 0, 0, 0, 1;
            //vec1 *= SuperMatrix;
            //vec1 = denormalizeVec3(vec1, 0, 541);
            vec1 *= ScaleMatrix*TriangleWorldMatrix* VeiwProjViewportMatrix;
            //vec1(0) += walk;


            //vec2 = normalizeVec3(vec2, 0, w);
            vec2 << 0.5, 0, 0, 1;
            //vec2 *= SuperMatrix;
            //vec2 = denormalizeVec3(vec2, 0, 541);
            vec2 *= ScaleMatrix*TriangleWorldMatrix*VeiwProjViewportMatrix;


            //vec3 = normalizeVec3(vec3, 0, w);
            vec3 << 0, 0.5, 0, 1;
            //vec3 *= SuperMatrix;
            //vec3 = denormalizeVec3(vec3, 0, 541);
            vec3 *= ScaleMatrix*TriangleWorldMatrix*VeiwProjViewportMatrix;

            // ---------------------------------------------------------
            //Recolor inner of rectangle
            newBrush = CreateSolidBrush(RGB(238, 155, 1));
            oldBrush = SelectObject(hdc, newBrush);
            // ---------------------------------------------------------
            vecBLR << 0, 0, 0, 1;
            vecBLRA << 1, 0, 0, 1;
            vecBLG << 0, 0, 0, 1;
            vecBLGA << 0, 1, 0, 1;
            vecBLB << 0, 0, 0, 1;
            vecBLBA << 0, 0, 1, 1;

            vecBLR *= ScaleMatrix*VeiwProjViewportMatrix;
            vecBLRA*= ScaleMatrix*VeiwProjViewportMatrix;
            vecBLG *= ScaleMatrix*VeiwProjViewportMatrix;
            vecBLGA*= ScaleMatrix*VeiwProjViewportMatrix;
            vecBLB *= ScaleMatrix*VeiwProjViewportMatrix;
            vecBLBA*= ScaleMatrix*VeiwProjViewportMatrix;

            /*if((vecBLR(2)>0) && (vecBLRA(2) > 0) && (vecBLGA(2) > 0) && (vecBLBA(2) > 0))*/ {
                hpen = CreatePen(PS_SOLID, 1, RGB(200, 65, 65));
                SelectObject(hdc, hpen);
                MoveToEx(hdc, vecBLR(0), vecBLR(1), NULL);
                LineTo(hdc, vecBLRA(0), vecBLRA(1));
                hpen = CreatePen(PS_SOLID, 1, RGB(76, 200, 38));
                SelectObject(hdc, hpen);
                MoveToEx(hdc, vecBLG(0), vecBLG(1), NULL);
                LineTo(hdc, vecBLGA(0), vecBLGA(1));
                hpen = CreatePen(PS_SOLID, 1, RGB(0, 69, 200));
                SelectObject(hdc, hpen);
                MoveToEx(hdc, vecBLB(0), vecBLB(1), NULL);
                LineTo(hdc, vecBLBA(0), vecBLBA(1));
            }

            /*for (size_t x = 0; x < vecsL1.size(); x++)
            {
                v = vecsL1[x];
                v *= CameraViewMatrix.inverse() * PerspectiveProjectionMatrix * DeViewPortMatrix3;
                a = v(0);
                b = v(1);
                
                v = vecsL1A[x];
                v *= CameraViewMatrix.inverse() * PerspectiveProjectionMatrix * DeViewPortMatrix3;
                c = v(0);
                d = v(1);
                MoveToEx(hdc, a, b, NULL);
                LineTo(hdc, c, d);
            }*/

            std::wostringstream wcharStream;
            wcharStream << "[" << vec1(0) << ", " << vec1(1) << "]" << ", " << "[" << vec2(0) << ", " << vec2(1) << "]" << ", " << "[" << vec3(0) << ", " << vec3(1) << "]";

            TextOut(hdc, 10, 10, wcharStream.str().c_str(), wcharStream.tellp());
            TextOut(hdc, 132, 513, L"Rotation: Q, Dilation: T, Reflection: E, Traslation: R", _tcslen(L"Rotation: Q, Dilation: W, Reflection: E, Traslation: R"));

            hpen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
            SelectObject(hdc, hpen);
            /*if ((vec1(2) > 0) && (vec2(2) > 0) && (vec3(2) > 0))*/ {
                DrawTriangle(hdc, vec1(0), vec1(1), vec2(0), vec2(1), vec3(0), vec3(1));
            }
            

            
            EndPaint(hWnd, &ps);
            //HDC hdc = GetDC(NULL);
            
            //ReleaseDC(NULL, hdc);
        }
            break;
    case WM_TIMER:
        //walk += 10;
        ++frameNum;
        radians = frameNum * (M_PI / 180.0);
        radYaw = 4* frameNum * (M_PI / 180.0);
        radPitch = 2 * frameNum * (M_PI / 180.0);
        radRoll = 10 * frameNum * (M_PI / 180.0);

        //SendMessage(hWnd, WM_PAINT, 0, 0);
        InvalidateRect(hWnd, NULL, TRUE);
        //RedrawWindow(hWnd, &clientRect, NULL, RDW_INTERNALPAINT);
        break;
    case WM_DESTROY:
        KillTimer(hWnd, 1);
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
}
     return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_JMISSION, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_JMISSION));
    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
}