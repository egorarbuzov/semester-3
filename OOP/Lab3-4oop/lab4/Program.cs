using System;

class MyArray
{
    private int[] myArr;
    public MyArray(int[] arr) { myArr = arr; }


    public static MyArray operator +(MyArray a, MyArray b)
    {
        if (!(a != null && b != null && a.myArr.Length == b.myArr.Length)) { return null; }
        int[] result = new int[a.myArr.Length];
        for (int i = 0; i < a.myArr.Length; i++)
        {
           result[i] = a.myArr[i] + b.myArr[i];
        }
        return new MyArray(result);
    }

    public static MyArray operator -(MyArray a, MyArray b)
    {
        if (!(a != null && b != null && a.myArr.Length == b.myArr.Length)) { return null; }
        int[] result = new int[a.myArr.Length];
        for (int i = 0; i < a.myArr.Length; i++)
        {
            result[i] = a.myArr[i] - b.myArr[i];
        }
        return new MyArray(result);
    }

    public static MyArray operator *(MyArray a, int n)
    {
        int[] result = new int[a.myArr.Length];
        for (int i = 0; i < a.myArr.Length; i++)
        {
            result[i] = a.myArr[i] * n;
        }
        return new MyArray(result);
    }

    public static bool operator ==(MyArray a, MyArray b)
    {
        if (ReferenceEquals(a, b)) return true;
        if (ReferenceEquals(a, null) || ReferenceEquals(b, null)) return false;

        return a.myArr.SequenceEqual(b.myArr);
    }

    public static bool operator !=(MyArray a, MyArray b)
    {
        return !(a == b);
    }

    public void Print()
    {
        Console.Write("[");
        for (int i = 0; i < myArr.Length; i++)
        {
            Console.Write(myArr[i]);
            if (i < myArr.Length - 1)
                Console.Write(", ");
        }
        Console.WriteLine("]");
    }
}

class Program
{
    static void Main()
    {
        MyArray a = new MyArray(new int[] { 1, 2, 3 });
        MyArray b = new MyArray(new int[] { 10, 20, 30 });
        MyArray c = new MyArray(new int[] { 1, 2, 3 });
        MyArray d = new MyArray(new int[] { 1, 2 });

        Console.Write("a = ");
        a.Print();

        Console.Write("b = ");
        b.Print();

        Console.Write("a + b = ");
        (a + b).Print();

        Console.Write("a - b = ");
        (a - b).Print();

        Console.Write("a * 5 = ");
        (a * 5).Print();

        Console.WriteLine("\na == c: " + (a == c));
        Console.WriteLine("a == b: " + (a == b));
        Console.WriteLine("a != b: " + (a != b));
        Console.WriteLine("a == d: " + (a == d));
        
            MyArray e = a + d;
        e.Print();
        
    }
}