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
LPDIRECT3D9		  d3d;		// the pointer to our Direct3D interface
LPDIRECT3DDEVICE9 d3ddev;   // the pointer to the device class

// function prototypes
void initD3D(HWND hWnd);    // sets up and initializes Direct3D
void render_frame(void);    // renders a single frame
void cleanD3D(void);		// closes Direct3D and releases memory
void init_graphics(void);   // 3D declarations

//#define CUSTOMFVF (D3DFVF_XYZRHW | D3DFVF_DIFFUSE)
#define CUSTOMFVF (D3DFVF_XYZ | D3DFVF_DIFFUSE)

struct CUSTOMVERTEX
{
    FLOAT x, y, z, rhw;		// from the D3DFVF_XYZRHW flag
    DWORD color;			// from the D3DFVF_DIFFUSE flag
};

LPDIRECT3DVERTEXBUFFER9 v_buffer = NULL;
LPDIRECT3DINDEXBUFFER9  i_buffer = NULL;

unsigned short NUM;



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

	d3ddev->SetRenderState(D3DRS_LIGHTING, false);			// turn off the 3D lighting
	d3ddev->SetRenderState(D3DRS_ZENABLE, true);			// turn on the z-buffer
	d3ddev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);	// show both sides of the triangles

	init_graphics();										// call the function to initialize the triangle
}

// this is the function used to render a single frame
void render_frame(int num)
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
/*
		{  2.5f, -3.0f, 0.0f, D3DCOLOR_XRGB(0,   0, 255), },
		{  0.0f,  3.0f, 0.0f, D3DCOLOR_XRGB(0, 255,   0), },
		{ -2.5f, -3.0f, 0.0f, D3DCOLOR_XRGB(255, 0,   0), },
*/
/*
		{ -3.0f, 3.0f, 0.0f, D3DCOLOR_XRGB(0, 0, 255), },
		{ 3.0f, 3.0f, 0.0f, D3DCOLOR_XRGB(0, 255, 0), },
		{ -3.0f, -3.0f, 0.0f, D3DCOLOR_XRGB(255, 0, 0), },
		{ 3.0f, -3.0f, 0.0f, D3DCOLOR_XRGB(0, 255, 255), },
*/
		{ -3.0f, 3.0f, -3.0f, D3DCOLOR_XRGB(0, 0, 255), },    // vertex 0
		{ 3.0f, 3.0f, -3.0f, D3DCOLOR_XRGB(0, 255, 0), },     // vertex 1
		{ -3.0f, -3.0f, -3.0f, D3DCOLOR_XRGB(255, 0, 0), },   // 2
		{ 3.0f, -3.0f, -3.0f, D3DCOLOR_XRGB(0, 255, 255), },  // 3
		{ -3.0f, 3.0f, 3.0f, D3DCOLOR_XRGB(0, 0, 255), },     // ...
		{ 3.0f, 3.0f, 3.0f, D3DCOLOR_XRGB(255, 0, 0), },
		{ -3.0f, -3.0f, 3.0f, D3DCOLOR_XRGB(0, 255, 0), },
		{ 3.0f, -3.0f, 3.0f, D3DCOLOR_XRGB(0, 255, 255), },
	};

	short indices[] =
	{
		0, 1, 2,    // side 1
		2, 1, 3,
		4, 0, 6,    // side 2
		6, 0, 2,
		7, 5, 6,    // side 3
		6, 5, 4,
		3, 1, 7,    // side 4
		7, 1, 5,
		4, 5, 0,    // side 5
		0, 5, 1,
		3, 7, 2,    // side 6
		2, 7, 6,
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

// this is the function used to render a single frame
void render_frame(void)
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

void render_frame3(void)
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
