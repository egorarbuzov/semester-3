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

public static class StringExtensions
{
    public static bool IsPalindrome(this string str)
    {
        if (string.IsNullOrEmpty(str)) return true;

        string cleaned = str.Replace(" ", "").ToLower();
        
        return cleaned == cleaned.Reverse();
    }

    public static int CountWords(this string str)
    {
        if (string.IsNullOrWhiteSpace(str)) return 0;

        return str.Split(' ', '.', ',', '!', '?')
            .Count(w => w.Length > 0);
    }

    public static string Reverse(this string str)
    {
        if (string.IsNullOrEmpty(str)) return str;

        char[] chars = str.ToCharArray();
        Array.Reverse(chars);
        return new string(chars);
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
        

        Console.WriteLine();
        
        string word = "шалаш";
        string text = "Hello, world! How are you?";

        Console.WriteLine($"{word} — палиндром? {word.IsPalindrome()}");
        Console.WriteLine($"Слов в '{text}': {text.CountWords()}");
        Console.WriteLine($"'{text}' наоборот: {text.Reverse()}");
        
    }
}