using System;

class Program
{
    static bool IsPalindrome(int number)
    {
        int value = Math.Abs(number);
        int reversed = 0;
        int temp = value;

        while (temp > 0)
        {
            reversed = reversed * 10 + temp % 10;
            temp /= 10;
        }

        return value == reversed;
    }

    static void Main()
    {
        int[] numbers = { 1, 3, 7, 9, 12, 15, 22, 27, 121, 4224, 33, 44, 8, 51 };

        Console.WriteLine("Исходный массив: " + string.Join(", ", numbers));

        int[] multiplesOfThree = Array.FindAll(numbers, x => x % 3 == 0);
        Console.WriteLine("Числа, кратные 3: " + string.Join(", ", multiplesOfThree));

        int[] doubled = Array.ConvertAll(multiplesOfThree, x => x * 2);
        Console.WriteLine("Увеличенные в 2 раза: " + string.Join(", ", doubled));

        Predicate<int> isPalindrome = x => x > 9 && IsPalindrome(x);

        int firstPalindrome = Array.Find(numbers, isPalindrome);

        if (firstPalindrome != 0)
            Console.WriteLine($"Первое число-палиндром в массиве: {firstPalindrome}");
        else
            Console.WriteLine("Чисел-палиндромов в массиве нет");
    }
}