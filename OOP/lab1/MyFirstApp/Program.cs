using System;

class Program
{
    static void Main()
    {
        bool b = true;
        byte by = 1;
        sbyte sbv = -1;
        char c = 'A';
        decimal dec = 1.5m;
        double dbl = 2.5;
        float fl = 3.5f;
        int i = 10;
        uint ui = 20;
        long l = 100;
        ulong ul = 200;
        short s = 5;
        ushort us = 15;

        Console.WriteLine($"bool={b}, byte={by}, sbyte={sbv}, char={c}");
        Console.WriteLine($"decimal={dec}, double={dbl}, float={fl}, int={i}");
        Console.WriteLine($"uint={ui}, long={l}, ulong={ul}, short={s}, ushort={us}");

        Console.Write("Введите int: ");
        i = Convert.ToInt32(Console.ReadLine());
        Console.Write("Введите short: ");
        s = Convert.ToInt16(Console.ReadLine());
        Console.WriteLine($"int={i}, short={s}");

        Console.WriteLine("Приведения:");
        long imp1 = i;
        double imp2 = fl;
        int imp3 = c;
        decimal imp4 = i;
        float imp5 = by;
        Console.WriteLine($"Неявные: {imp1}, {imp2}, {imp3}, {imp4}, {imp5}");

        int exp1 = (int)dbl;
        int exp2 = (int)l;
        int exp3 = (int)fl;
        byte exp4 = (byte)i;
        char exp5 = (char)i;
        Console.WriteLine($"Явные: {exp1}, {exp2}, {exp3}, {exp4}, {exp5}");

        string numStr = "42";
        int fromStr = Convert.ToInt32(numStr);
        double fromStrD = Convert.ToDouble("3,14");
        Console.WriteLine($"Convert.ToInt32: {fromStr}, Convert.ToDouble: {fromStrD}");

        Console.WriteLine("\nУпаковка и распаковка");
        int value_to_box = 123;
        object value_boxed = value_to_box;
        int value_unboxed = (int)value_boxed;
        Console.WriteLine($"Упаковка: {value_boxed}, Распаковка: {value_unboxed}");

        Console.WriteLine("\nНеявно типизированная переменная:");
        var implicitVar = 100;
        Console.WriteLine($"var = {implicitVar}, тип = {implicitVar.GetType()}");

        Console.WriteLine("\nNullable:");
        int? nullableInt = null;
        Console.WriteLine($"HasValue: {nullableInt.HasValue}");
        nullableInt = 5;
        Console.WriteLine($"Value: {nullableInt.Value}");

        Console.WriteLine("\nОшибка var:");
        var v = 10;
        // v = "hello";
        Console.WriteLine("Ошибка: var определяет тип один раз при инициализации и не позволяет менять его.");

        Console.WriteLine("2. Строки");
        Console.WriteLine("Сравнение строк:");
        string first_str = "hello1";
        string second_str = "hello2";
        string third_str = "hello1";
        Console.WriteLine(first_str == second_str);
        Console.WriteLine(first_str == third_str);

        Console.WriteLine("Операции со строками:");
    }
}