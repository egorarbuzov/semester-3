using System;

class Dog
{
    private string name;
    private string breed;
    private int age;

    public Dog(string name, string breed, int age = 1)
    {
        this.name = name;
        this.breed = breed;
        this.age = age;
    }
    public void Bark()
    {
        Console.WriteLine($"Гав! Меня зовут {name}");
    }

    public void DisplayInfo()
    {
        Console.WriteLine($"Имя: {name}");
        Console.WriteLine($"Порода: {breed}");
        Console.WriteLine($"Возраст: {age}");
    }
}

class Program
{
    static void Main(string[] args)
    {
        Dog dog1 = new Dog("Собака 1", "Порода 1");
        Dog dog2 = new Dog("Крутая собака 2", "Порода 2", 8);
        dog1.Bark();
        dog2.Bark();
        dog1.DisplayInfo();
        dog2.DisplayInfo();
    }
}