// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� SENSECRYPTDLL_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// SENSECRYPTDLL_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifdef SENSECRYPTDLL_EXPORTS
#define SENSECRYPTDLL_API extern "C" _declspec(dllexport)
#else
#define SENSECRYPTDLL_API extern "C" __declspec(dllimport)
#endif

SENSECRYPTDLL_API int S4RSAPublicEncrypt(
			unsigned char *output,
			unsigned char *input,
			unsigned int inputLen
		  );