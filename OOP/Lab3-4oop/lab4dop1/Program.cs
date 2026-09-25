using System;

class SmartArray
{
    private int[] myArr;

    public SmartArray(int[] arr)
    {
        myArr = arr;
    }

    public int Length
    {
        get { return myArr.Length; }
    }
    
    public int this[int index]
    {
        get
        {
            if (index < 1 || index > myArr.Length)
                return 0;
            return myArr[index - 1];
        }
        set
        {
            if (index < 1 || index > myArr.Length)
                return;
            myArr[index - 1] = value;
        }
    }

    public static SmartArray operator +(SmartArray arr, int value)
    {
        int[] result = new int[arr.myArr.Length];
        for (int i = 0; i < arr.myArr.Length; i++)
        {
            result[i] = arr.myArr[i] + value;
        }
        return new SmartArray(result);
    }
    
    public static SmartArray operator -(SmartArray arr, int value)
    {
        int[] result = new int[arr.myArr.Length];
        for (int i = 0; i < arr.myArr.Length; i++)
        {
            result[i] = arr.myArr[i] - value;
        }
        return new SmartArray(result);
    }

    public void PrintArray()
    {
        Console.Write("Массив: [ ");
        foreach (int i in myArr)
        {
            Console.Write($"{i} ");
        }
        Console.Write("]");
        Console.WriteLine();
    }
}

static class SmartArrayExtensions
{
    private static readonly Random random = new Random();

    public static void Shuffle(this SmartArray arr)
    {
        for (int i = arr.Length; i > 1; i--)
        {
            int j = random.Next(1, i + 1);
            int temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }
}

class Program
{
    static void Main(string[] args)
    {
        int[] myArr = { 0, 1, 2 };
        SmartArray newArr = new SmartArray(myArr);

        Console.WriteLine("Исходный массив:");
        newArr.PrintArray();
        
        Console.WriteLine($"\nЭлемент с индексом 1: {newArr[1]}");
        Console.WriteLine($"Элемент с индексом 3: {newArr[3]}");
        Console.WriteLine($"Элемент с индексом 10 (вне диапазона): {newArr[10]}"); // вернёт 0
        
        SmartArray plusArr = newArr + 3;
        Console.WriteLine("\nПосле arr + 3:");
        plusArr.PrintArray();
        
        SmartArray minusArr = newArr - 2;
        Console.WriteLine("\nПосле arr - 2:");
        minusArr.PrintArray();
        
        Console.WriteLine("\nПеремешивание 1:");
        newArr.Shuffle();
        newArr.PrintArray();

        Console.WriteLine("\nПеремешивание 2:");
        newArr.Shuffle();
        newArr.PrintArray();

        Console.WriteLine("\nПеремешивание 3");
        newArr.Shuffle();
        newArr.PrintArray();
    }
}