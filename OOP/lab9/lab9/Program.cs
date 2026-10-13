using System;

class NegativeNumberException : Exception
{
    public double Number { get; set; }

    public NegativeNumberException() : base() { }

    public NegativeNumberException(string message) : base(message) { }

    public NegativeNumberException(string message, Exception inner) : base(message, inner) { }
}

class Program
{
    static void Divide()
    {
        Console.WriteLine("Деление чисел");

        bool success = false;

        while (!success)
        {
            try
            {
                Console.Write("Введите первое число: ");
                string input1 = Console.ReadLine();
                int a = int.Parse(input1);

                Console.Write("Введите второе число: ");
                string input2 = Console.ReadLine();
                int b = int.Parse(input2);

                int result = a / b;
                Console.WriteLine($"Результат: {a} / {b} = {result}");
                success = true;
            }
            catch (DivideByZeroException ex)
            {
                Console.WriteLine($"Ошибка: деление на ноль! {ex.Message}");
            }
            catch (FormatException ex)
            {
                Console.WriteLine($"Ошибка: неверный формат числа! {ex.Message}");
            }
            finally
            {
                Console.WriteLine("Блок finally выполнен (очистка ресурсов)");
            }
        }
    }
    
    static double CalculateSquareRoot(double number)
    {
        if (number < 0)
        {
            NegativeNumberException ex = new NegativeNumberException($"Нельзя взять корень из отрицательного числа. Получено: {number}");
            ex.Number = number;
            throw ex;
        }
        else
        {
            return Math.Sqrt(number);
        }
    }

    static void Main(string[] args)
    {
        Divide();

        Console.WriteLine();
        
        Console.WriteLine("Квадратный корень");

        try
        {
            Console.WriteLine($"Корень из 25: {CalculateSquareRoot(25)}");
            Console.WriteLine($"Корень из 2: {CalculateSquareRoot(2)}");
            Console.WriteLine($"Корень из -9: {CalculateSquareRoot(-9)}");
        }
        catch (NegativeNumberException ex)
        {
            Console.WriteLine($"Перехвачено исключение: {ex.Message}");
            Console.WriteLine($"Некорректное число: {ex.Number}");
        }
    }
}