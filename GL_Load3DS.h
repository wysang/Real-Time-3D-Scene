#ifndef GL_LOAD3DS_H
#define GL_LOAD3DS_H

#include "GL_Vector3.h"
#include "GL_Texture.h"
#include <math.h>
#include <vector>
using namespace std;

#define  BYTE unsigned char
#define  WORD unsigned short 
#define  UINT unsigned int

// ����3ds��һЩ��ʹ�õ���ChunkID
// ��Chunk,��ÿ���ļ��Ŀ�ʼλ��
const WORD PRIMARY=0x4D4D;
	const WORD PRIM_EDIT = 0x3D3D;					// ChunkID��3dsģ��
		const WORD EDIT_MAT = 0xAFFF;				// ChunkID������
			const WORD MAT_NAME = 0xA000;			// ChunkID����������
			const WORD MAT_AMB  = 0xA010;			// ChunkID�����ʻ��������ԣ�ûʹ�õ���
			const WORD MAT_DIF  = 0xA020;			// ChunkID����������������
			const WORD MAT_SPE  = 0xA030;			// ChunkID�����ʾ��淴�����ԣ�ûʹ�õ���
			const WORD MAT_MAP =  0xA200;			// ChunkID�����ʵ�����
				const WORD MAP_NAME = 0xA300;		// ChunkID������������

		const WORD EDIT_OBJECT = 0x4000;			// ChunkID��3ds������桢�����Ϣ
			const WORD OBJECT_INFO = 0x4100;		// ChunkID���������Ҫ��Ϣ
				const WORD OBJECT_VERTEX = 0x4110;	// ChunkID������Ķ�����Ϣ
				const WORD OBJECT_FACET = 0x4120;	// ChunkID�����������Ϣ
					const WORD FACET_MAT = 0x4130;	// ChunkID��������еĲ���
					const WORD FACET_SMOOTH =0x4150;// ChunkID����⻬��Ϣ��ûʹ�õ���
				const WORD OBJECT_UV = 0x4140;		// ChunkID������������Ϣ
				const WORD OBJECT_LOCAL = 0x4160;
	const WORD PRIM_KEY=0xB000;						// ChunkID�����еĹؼ�֡��Ϣ��ûʹ�õ���

const WORD COLOR_BYTE=0x0011;						// ChunkID����ɫ

// �����ַ���
typedef struct
{
	char string[128];
} STRING;

// ����Chunk��Ϣ
typedef struct 
{
	WORD ID;										// Chunk��ID
	UINT length;									// Chunk�ĳ���
} tChunk;

// ��������Ϣ���������������������ֵ��
typedef struct
{
	int vertIndex[3];								// 3�����������ֵ
	int	matID;										// �����Ӧ�Ĳ���ID
} tFace;

// ���������Ϣ��
typedef struct
{
	STRING  matName;								// ���ʵ�����
	STRING  mapName;								// ���������ƣ�bmp��jpg�ȵ��ļ�����
	BYTE	color[3];								// ������ɫ
	UINT	texureId;								// ������ID��ָ�������������
	bool	isTexMat;								// �ò����ǲ��ǰ���������
} tMaterial;

// ���浥��3ds����
typedef struct
{
	int  numOfVerts;								// �ö��󶥵�ĸ���
	int  numOfFaces;								// �ö�����ĸ���
	int  numTexVertex;								// �ö�����������ĸ���
	STRING	 objName;								// ������������
	GL_Vector3  *pVerts;								// ���涥������
	GL_Vector3  *pNormals;								// �����ķ�����
	Texcoord  *pTexVerts;							// ������������
	tFace	 *pFaces;								// ��������Ϣ���������������Ӧ�Ĳ��ʣ�
} t3DObject;

// ��������3dsģ��
typedef struct
{
	int numOfObjects;								// 3ds����ĸ���
	int numOfMaterials;								// 3ds���ʵĸ���
	vector<tMaterial> pMaterials;					// ����3ds����
	vector<t3DObject> pObject;						// ����3ds����
} t3DModel;

// C3DSModel��
class C3DSModel
{
public:
	C3DSModel();
	~C3DSModel();
	BOOL Load(char *,char *);						// ����3ds�ļ�
	void Render(void);								// ����3dsģ��
	void Release(void);								// �ͷ�3dsģ����Դ
	void show3ds(int ,float ,float ,float ,float );
	void Scene(float ,float ,float ,float  ,float ,float ,float ,float );
private:
	void ReadChunk(tChunk *);						// ��ȡ3ds��һ��Chunk��Ϣ��Chunk��ID�����ȣ�

	UINT ReadPrimary(UINT n);						// ��ȡ3ds�ļ���ҪChunk
		UINT ReadEdit(UINT n);						// ��ȡ3ds�������༭Chunk
			UINT ReadObject(UINT n);						// ��ȡ3ds����
				UINT ReadObjectInfo(t3DObject *,UINT n);	// ��ȡ3ds������Ϣ
					UINT ReadFacetInfo(t3DObject *,UINT n);	// ��ȡ����Ϣ
		UINT ReadMaterial(UINT n);					// ��ȡ����
			UINT ReadMatDif(tMaterial *, UINT n);	// ��ȡ���ʵ�����������
			UINT ReadMatMap(tMaterial *, UINT n);	// ��ȡ���ʵ�����
		UINT ReadKeyframe(UINT n);					// ��ȡ֡��Ϣ��δʹ�ã�

	BYTE ReadByte(void);							// ���ļ��ж�ȡ1���ֽ�
	WORD ReadWord(void);							// ���ļ��ж�ȡ2���ֽ�
	UINT ReadUint(void);							// ���ļ��ж�ȡ4���ֽ�
	float ReadFloat(void);							// ���ļ��ж�ȡ������
	UINT ReadString(STRING *);						// ���ļ��ж�ȡ�ַ����������ַ������ȣ�
    void ComputeNormals(void);
private:
	FILE *m_FilePtr;								// 3ds�ļ�ָ��
	t3DModel m_3DModel;								// ����3dsģ��
	GL_Texture m_texture;
};

#endif       //GL_LOAD3DS_H