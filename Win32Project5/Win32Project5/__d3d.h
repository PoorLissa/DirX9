// include the Direct3D Library file
#include "d3d9.h"
#include "d3dx9.h"
//#include "d3dx9math.h"

#ifdef _DEBUG
#   pragma comment(lib, "d3dx9d.lib")
#else
#   pragma comment(lib, "d3dx9.lib")
#endif

#pragma comment(lib, "d3d9.lib")
// --------------------------------------------------------------------------------------

// global declarations
LPDIRECT3D9		  d3d;										// the pointer to our Direct3D interface
LPDIRECT3DDEVICE9 d3ddev;									// the pointer to the device class

// function prototypes
void initD3D(HWND hWnd);									// sets up and initializes Direct3D
void render_frame(void);									// renders a single frame
void cleanD3D(void);										// closes Direct3D and releases memory
void init_graphics(void);									// 3D declarations
void init_directional_light();								// sets up the directional light and the material
void init_point_light(D3DXCOLOR, D3DXVECTOR3, int);			// sets up the point light and the material
void init_spot_light(D3DXCOLOR, D3DXVECTOR3, D3DXVECTOR3, int);

//#define CUSTOMFVF (D3DFVF_XYZRHW | D3DFVF_DIFFUSE)
#define CUSTOMFVF (D3DFVF_XYZ | D3DFVF_DIFFUSE)
//#define CUSTOMFVF (D3DFVF_XYZ | D3DFVF_NORMAL)
struct CUSTOMVERTEX
{
    FLOAT x, y, z, rhw;					// from the D3DFVF_XYZRHW flag
    DWORD color;						// from the D3DFVF_DIFFUSE flag
	//D3DVECTOR Normal;
};

LPDIRECT3DVERTEXBUFFER9 v_buffer = NULL;
LPDIRECT3DINDEXBUFFER9  i_buffer = NULL;

unsigned short NUM;
extern int xPos, yPos, zPos;



// this function initializes and prepares Direct3D for use
void initD3D(HWND hWnd, int Width, int Height)
{
    d3d = Direct3DCreate9(D3D_SDK_VERSION);		// create the Direct3D interface

    D3DPRESENT_PARAMETERS d3dpp;				// create a struct to hold various device information

    ZeroMemory(&d3dpp, sizeof(d3dpp));			// clear out the struct for use
#if defined fullScreen
    d3dpp.Windowed = false;						// program windowed, not fullscreen
#else
	d3dpp.Windowed = true;						// program windowed, not fullscreen
#endif
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;   // discard old frames
    d3dpp.hDeviceWindow = hWnd;					// set the window to be used by Direct3D

	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;    // set the back buffer format to 32-bit
    d3dpp.BackBufferWidth = Width;				// set the width of the buffer
    d3dpp.BackBufferHeight = Height;			// set the height of the buffer
	// turn z-Buffering ON:
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

    // create a device class using this information and information from the d3dpp stuct
    d3d->CreateDevice(D3DADAPTER_DEFAULT,
                      D3DDEVTYPE_HAL,
                      hWnd,
                      D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                      &d3dpp,
                      &d3ddev);

	d3ddev->SetRenderState(D3DRS_LIGHTING, true);							// turn off the 3D lighting
	d3ddev->SetRenderState(D3DRS_ZENABLE, true);							// turn on the z-buffer
	d3ddev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);					// show both sides of the triangles
	d3ddev->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_XRGB(50, 50, 50));		// ambient light
	d3ddev->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);					// automatically handle the normals depending on scale (scaling problem lighness fix)

	d3ddev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);					// Enable alpha blending
	d3ddev->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);					// type of blending operation
	d3ddev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	d3ddev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	init_graphics();														// call the function to initialize the triangle
	//init_directional_light();
	//init_point_light(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f), D3DXVECTOR3(0.0f, 5.0f, 0.0f));
	//init_spot_light(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f), D3DXVECTOR3(-12.0f, 0.0f, 30.0f), D3DXVECTOR3(0.0f, 0.0f, -1.0f));
}

// this is the function that cleans up Direct3D and COM
void cleanD3D(void)
{
	if( v_buffer != NULL)
		v_buffer->Release();    // close and release the vertex buffer

    d3ddev->Release();  // close and release the 3D device
    d3d->Release();		// close and release Direct3D
}

void init_graphics(void)
{
    // create three vertices using the CUSTOMVERTEX struct built earlier
	CUSTOMVERTEX vertices[] =
	{
		{ -3.0f, -3.0f, 3.0f, D3DCOLOR_ARGB(255, 0, 0, 255), },    // side 1
		{ 3.0f, -3.0f, 3.0f, D3DCOLOR_ARGB(255, 0, 0, 255), },
		{ -3.0f, 3.0f, 3.0f, D3DCOLOR_ARGB(255, 0, 0, 255), },
		{ 3.0f, 3.0f, 3.0f, D3DCOLOR_ARGB(255, 0, 0, 255), },

		{ -3.0f, -3.0f, -3.0f, D3DCOLOR_ARGB(255, 0, 0, 255), },    // side 2
		{ -3.0f, 3.0f, -3.0f,  D3DCOLOR_ARGB(255, 0, 0, 255), },
		{ 3.0f, -3.0f, -3.0f,  D3DCOLOR_ARGB(255, 0, 0, 255), },
		{ 3.0f, 3.0f, -3.0f,   D3DCOLOR_ARGB(255, 0, 0, 255), },

		{ -3.0f, 3.0f, -3.0f, D3DCOLOR_ARGB(255, 0, 0, 255), },    // side 3
		{ -3.0f, 3.0f, 3.0f, D3DCOLOR_ARGB(255, 0, 0, 255), },
		{ 3.0f, 3.0f, -3.0f, D3DCOLOR_ARGB(255, 0, 0, 255), },
		{ 3.0f, 3.0f, 3.0f, D3DCOLOR_ARGB(255, 0, 0, 255), },

		{ -3.0f, -3.0f, -3.0f, D3DCOLOR_ARGB(255, 0, 0, 255), },    // side 4
		{ 3.0f, -3.0f, -3.0f, D3DCOLOR_ARGB(255, 0, 0, 255), },
		{ -3.0f, -3.0f, 3.0f, D3DCOLOR_ARGB(255, 0, 0, 255), },
		{ 3.0f, -3.0f, 3.0f, D3DCOLOR_ARGB(255, 0, 0, 255), },

		{ 3.0f, -3.0f, -3.0f, D3DCOLOR_ARGB(255, 0, 0, 255), },    // side 5
		{ 3.0f, 3.0f, -3.0f, D3DCOLOR_ARGB(255, 0, 0, 255), },
		{ 3.0f, -3.0f, 3.0f, D3DCOLOR_ARGB(255, 0, 0, 255), },
		{ 3.0f, 3.0f, 3.0f, D3DCOLOR_ARGB(255, 0, 0, 255), },

		{ -3.0f, -3.0f, -3.0f, D3DCOLOR_ARGB(255, 0, 0, 255), },    // side 6
		{ -3.0f, -3.0f, 3.0f, D3DCOLOR_ARGB(255, 0, 0, 255), },
		{ -3.0f, 3.0f, -3.0f, D3DCOLOR_ARGB(255, 0, 0, 255), },
		{ -3.0f, 3.0f, 3.0f, D3DCOLOR_ARGB(255, 0, 0, 255), },
	};

	short indices[] =
	{
		0, 1, 2,    // side 1
		2, 1, 3,
		4, 5, 6,    // side 2
		6, 5, 7,
		8, 9, 10,    // side 3
		10, 9, 11,
		12, 13, 14,    // side 4
		14, 13, 15,
		16, 17, 18,    // side 5
		18, 17, 19,
		20, 21, 22,    // side 6
		22, 21, 23,
	};

	NUM = sizeof(vertices) / sizeof(vertices[0]);

    // create the vertex and store the pointer into v_buffer, which is created globally
    d3ddev->CreateVertexBuffer(NUM * sizeof(CUSTOMVERTEX),
                               0,
                               CUSTOMFVF,
                               D3DPOOL_MANAGED,
                               &v_buffer,
                               NULL
	);

	d3ddev->CreateIndexBuffer(3 * 12 * sizeof(short),    // 3 per triangle, 12 triangles in a cube
							0,
							D3DFMT_INDEX16,
							D3DPOOL_MANAGED,
							&i_buffer,
							NULL
	);

    VOID* pVoid;    // the void pointer

    v_buffer->Lock(0, 0, (void**)&pVoid, 0);		// lock the vertex buffer
    memcpy(pVoid, vertices, sizeof(vertices));		// copy the vertices to the locked buffer
    v_buffer->Unlock();								// unlock the vertex buffer

	// lock i_buffer and load the indices into it
	i_buffer->Lock(0, 0, (void**)&pVoid, 0);
	memcpy(pVoid, indices, sizeof(indices));
	i_buffer->Unlock();
}

// this is the function that sets up the lights and materials
void init_directional_light(void)
{
	D3DLIGHT9 light;											// create the light struct
	D3DMATERIAL9 material;										// create the material struct

	ZeroMemory(&light, sizeof(light));							// clear out the light struct for use
	light.Type = D3DLIGHT_DIRECTIONAL;							// make the light type 'directional light'
	light.Diffuse = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);			// set the light's color
	light.Direction = D3DXVECTOR3(-1.0f, -0.3f, -1.0f);

	d3ddev->SetLight(0, &light);								// send the light struct properties to light #0
	d3ddev->LightEnable(0, TRUE);								// turn on light #0

	ZeroMemory(&material, sizeof(D3DMATERIAL9));				// clear out the struct for use
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// set diffuse color to white
	material.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// set ambient color to white

	d3ddev->SetMaterial(&material);								// set the globably-used material to &material
}

void init_point_light(D3DXCOLOR Color, D3DXVECTOR3 Vector, int LightNo = -1)
{
	static unsigned int lightNo = 500;
	D3DLIGHT9 light;
	D3DMATERIAL9 material;

	if (LightNo >= 0)
		lightNo = LightNo;

	ZeroMemory(&light, sizeof(light));
	light.Type = D3DLIGHT_POINT;		// make the light type 'point light'
	light.Diffuse = Color;
	light.Position = Vector;
	light.Range = 100.0f;				// a range of 100
	light.Attenuation0 = 0.0f;			// no constant inverse attenuation
	light.Attenuation1 = 0.125f;		// only .125 inverse attenuation
	light.Attenuation2 = 0.0f;			// no square inverse attenuation

	d3ddev->SetLight(lightNo, &light);
	d3ddev->LightEnable(lightNo, TRUE);

	ZeroMemory(&material, sizeof(D3DMATERIAL9));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	material.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	d3ddev->SetMaterial(&material);

	lightNo++;
}

void init_spot_light(D3DXCOLOR Color, D3DXVECTOR3 VectorPos, D3DXVECTOR3 VectorDir, int LightNo = -1)
{
	static unsigned int lightNo = 1000;
	D3DLIGHT9 light;
	D3DMATERIAL9 material;

	if (LightNo >= 0)
		lightNo = LightNo;

	ZeroMemory(&light, sizeof(light));
	light.Type = D3DLIGHT_SPOT;    // make the light type 'spot light'
	light.Diffuse = Color;
	light.Position = VectorPos;
	light.Direction = VectorDir;
	light.Range = 100.0f;    // a range of 100
	light.Attenuation0 = 0.0f;    // no constant inverse attenuation
	light.Attenuation1 = 0.125f;    // only .125 inverse attenuation
	light.Attenuation2 = 0.0f;    // no square inverse attenuation
	light.Phi = D3DXToRadian(40.0f);    // set the outer cone to 30 degrees
	light.Theta = D3DXToRadian(20.0f);    // set the inner cone to 10 degrees
	light.Falloff = 1.0f;    // use the typical falloff

	d3ddev->SetLight(lightNo, &light);
	d3ddev->LightEnable(lightNo, TRUE);

	ZeroMemory(&material, sizeof(D3DMATERIAL9));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	material.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	d3ddev->SetMaterial(&material);

	lightNo++;
}

// this is the function used to render a single frame
void render_frame0(int num)
{
	// clear the window to a deep blue
	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 40, 100), 1.0f, 0);

	// clear our z-buffer
	d3ddev->Clear(0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

	// begins the 3D scene
	d3ddev->BeginScene();

	// do 3D rendering on the back buffer here

	d3ddev->EndScene();    // ends the 3D scene

	d3ddev->Present(NULL, NULL, NULL, NULL);    // displays the created frame
}

// this is the function used to render a single frame
void render_frame1()
{
    d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 40, 100), 1.0f, 0);

    d3ddev->BeginScene();

        // select which vertex format we are using
        d3ddev->SetFVF(CUSTOMFVF);

        // select the vertex buffer to display
        d3ddev->SetStreamSource(0, v_buffer, 0, sizeof(CUSTOMVERTEX));

        // copy the vertex buffer to the back buffer
        d3ddev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);
		
    d3ddev->EndScene();

    d3ddev->Present(NULL, NULL, NULL, NULL);
}

void render_frame2()
{
	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 40, 100), 1.0f, 0);
	d3ddev->Clear(0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

	d3ddev->BeginScene();


		// select which vertex format we are using
		d3ddev->SetFVF(CUSTOMFVF);

		// SET UP THE PIPELINE

		D3DXMATRIX matView;											// the view transform matrix
		D3DXMatrixLookAtLH(&matView,
			&D3DXVECTOR3(0.0f, 0.0f, 20.0f),						// the camera position
			&D3DXVECTOR3(0.0f, 0.0f, 0.0f),						// the look-at position
			&D3DXVECTOR3(0.0f, 1.0f, 0.0f));						// the up direction
		d3ddev->SetTransform(D3DTS_VIEW, &matView);					// set the view transform to matView


		D3DXMATRIX matProjection;									// the projection transform matrix
		D3DXMatrixPerspectiveFovLH(
			&matProjection,
			D3DXToRadian(45),										// the horizontal field of view
			(FLOAT)SCREEN_WIDTH / (FLOAT)SCREEN_HEIGHT,				// aspect ratio
			1.0f,													// the near view-plane
			100.0													// the far view-plane
		);
		d3ddev->SetTransform(D3DTS_PROJECTION, &matProjection);		// set the projection


		// select the vertex buffer to display
		d3ddev->SetStreamSource(0, v_buffer, 0, sizeof(CUSTOMVERTEX));
		d3ddev->SetIndices(i_buffer);


		D3DXMATRIX matRotateX, matRotateY;

		static float index = 0.0f;
		index += 0.05f;

		D3DXMatrixRotationX(&matRotateX, index);
		D3DXMatrixRotationY(&matRotateY, index);

		// tell Direct3D about each world transform, and then draw another triangle
		d3ddev->SetTransform(D3DTS_WORLD, &(matRotateX * matRotateY));

		// draw the cube
		d3ddev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12);
		d3ddev->DrawPrimitive(D3DPT_LINESTRIP, 0, NUM);

/*
		D3DXMATRIX matTranslateA;		// a matrix to store the translation for triangle A
		D3DXMATRIX matTranslateB;		// a matrix to store the translation for triangle B
		D3DXMATRIX matRotateY;			// a matrix to store the rotation for each triangle

		static float index = 0.0f;
		index += 0.05f; // an ever-increasing float value


		// build MULTIPLE matrices to translate the model and one to rotate
		D3DXMatrixTranslation(&matTranslateA, 0.0f, 0.0f,  5.0f);
		D3DXMatrixTranslation(&matTranslateB, 0.0f, 0.0f, -5.0f);
		D3DXMatrixRotationY(&matRotateY, index);					// the front side


		// tell Direct3D about each world transform, and then draw another triangle
		d3ddev->SetTransform(D3DTS_WORLD, &(matTranslateA * matRotateY));
		d3ddev->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
		d3ddev->DrawPrimitive(D3DPT_LINESTRIP, 0, NUM);

		d3ddev->SetTransform(D3DTS_WORLD, &(matTranslateB * matRotateY));
		d3ddev->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
		d3ddev->DrawPrimitive(D3DPT_LINESTRIP, 0, NUM);
*/

	d3ddev->EndScene();

	d3ddev->Present(NULL, NULL, NULL, NULL);

	return;
}

void render_frame3()
{
	static float index = 0.0;
	index += 0.002;

	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
	d3ddev->Clear(0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

	init_point_light(D3DXCOLOR(0.5f, 0.9f, 0.5f, 1.0f), D3DXVECTOR3(0.0f + 10*sin(50 * index), 5.0f + 10*cos(50 * index), 5.0f), 10);
	init_spot_light(D3DXCOLOR(0.9f, 0.5f, 0.5f, 1.0f),
					//D3DXVECTOR3(0.0f + 10 * sin(50 * index), 5.0f + 10 * cos(50 * index), 5.0f),
					D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXVECTOR3(0.0f + 10 * sin(50 * index), 5.0f + 10 * cos(50 * index), 0.0f),
					11);

	d3ddev->BeginScene();

	{
		// select which vertex format we are using
		d3ddev->SetFVF(CUSTOMFVF);

		// set the view transform
		D3DXMATRIX matView;
		D3DXMatrixLookAtLH(&matView,
			&D3DXVECTOR3(0.0f, 0.0f, 50.0f + float(zPos)/50),   // the camera position + mouse wheel zoom
			&D3DXVECTOR3(0.0f, 0.0f, 0.0f),						// the look-at position
			&D3DXVECTOR3(0.0f, 1.0f, 0.0f));					// the up direction
		d3ddev->SetTransform(D3DTS_VIEW, &matView);

		// set the projection transform
		D3DXMATRIX matProjection;
		D3DXMatrixPerspectiveFovLH(&matProjection,
			D3DXToRadian(45),
			(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
			1.0f,												// the near view-plane
			100.0f + float(zPos)/50);							// the far view-plane
		d3ddev->SetTransform(D3DTS_PROJECTION, &matProjection);

		// select the vertex and index buffers to use
		d3ddev->SetStreamSource(0, v_buffer, 0, sizeof(CUSTOMVERTEX));
		d3ddev->SetIndices(i_buffer);

		for (int i = 0; i < 23; i++) {

			D3DXMATRIX matTranslation;
			D3DXMatrixTranslation(&matTranslation, float(10 * sin(i*i)), float(10 * cos(i*i)), float(25*sin(i) + 25*cos(i)));

			D3DXMATRIX matRotateX;
			D3DXMatrixRotationX(&matRotateX, float(yPos) / 300);

			D3DXMATRIX matRotateY;
			D3DXMatrixRotationY(&matRotateY, index + float(xPos) / 300);
			d3ddev->SetTransform(D3DTS_WORLD, &(matTranslation * matRotateX * matRotateY));

			// draw the cube
			d3ddev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 24, 0, 12);
		}
	}

	d3ddev->EndScene();

	d3ddev->Present(NULL, NULL, NULL, NULL);
}