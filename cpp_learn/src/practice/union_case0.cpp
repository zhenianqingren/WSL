#include <iostream>
union UNION
{
    struct
    {
        int x;
        int y;
        int z;
    } u;
    int k;
} a;

int main(int argc, char const *argv[])
{
    a.u.x = 4;
    a.u.y = 5;
    a.u.z = 6;
    a.k = 0;
    std::cout << a.u.x << "  " << a.u.y << "  " << a.u.z << "  " << a.k << std::endl;
    //������Ա��ռ��СΪ�ռ�
    //�ռ������ �����ֵ�����¸��ǵ�ǰ���ֵ
    return 0;
}
// Ĭ�Ϸ��ʿ��Ʒ�Ϊ public
// ���Ժ��й��캯������������
// ���ܺ����������͵ĳ�Ա
// ���ܼ̳��������࣬������Ϊ����
// ���ܺ����麯��
// ���� union �ڶ��������������ֱ�ӷ��� union ��Ա
// ���� union ���ܰ��� protected ��Ա�� private ��Ա
// ȫ���������ϱ����Ǿ�̬��static����
