using System;

interface IMovable
{
    decimal Speed { get; set; }
    decimal Position { get; set; }
    void Move(decimal time);
}

interface IRefuelable
{
    void Refuel(decimal amount);
}

class Car : IMovable, IRefuelable 
{
    public string Name { get; set; }
    public decimal Speed { get; set;  }
    public decimal Position { get; set; }
    public decimal Fuel { get; set; }

    public Car(decimal speed, string name)
    {
        if (speed > 300)
        {
            throw new Exception("Слишком быстро для автомобиля");
        }
        else
        {
            this.Speed = speed;
        }
        this.Name = name;
        this.Position = 0;
    }
    
    public void Move(decimal time)
    {
        if (time <= 0)
        {
            throw new Exception("Время меньше или равно 0");
        }
        else
        {
            if (Fuel >= time / 2)
            {
                this.Fuel -= time / 2;
                this.Position += time * this.Speed;
            }
            else
            {
                Console.WriteLine("Недостаточно бензина для перодоления этой дистанции");
            }

        }
    }

    public void Refuel(decimal amount)
    {
        if (amount >= 0)
        {
            this.Fuel += amount;
        }
        else
        {
            throw new Exception("Бензина не может быть меньше или равно 0");
        }
    }
}

class Bicycle : IMovable
{
    public string Name { get; set; }
    public decimal Speed { get; set; }
    public decimal Position { get; set; }

    public Bicycle(decimal speed, string name)
    {
        if (speed > 30)
        {
            throw new Exception("Слишком быстро для велосипеда");
        }
        else
        {
            this.Speed = speed;
        }
        this.Name = name;
        this.Position = 0;
    }
    
    public void Move(decimal time)
    {
        if (time <= 0)
        {
            throw new Exception("Время меньше или равно 0");
        }
        else
        {
            this.Position += time * this.Speed;
        }
    }
}

class Program
{
    static void Main(string[] args)
    {
        IMovable[] movables = new IMovable[2];
        movables[0] = new Car(200, "Car");
        movables[1] = new Bicycle(15, "Bicycle");
        
        if (movables[0] is IRefuelable carRefuel)
        {
            carRefuel.Refuel(100);
        }
        
        IRefuelable refuelable = movables[1] as IRefuelable;
        if (refuelable != null)
        {
            refuelable.Refuel(50);
        }
        else
        {
            Console.WriteLine("Велосипед заправлять не нужно");
        }
        
        Console.WriteLine(movables[0].Position);
        Console.WriteLine(movables[1].Position);
        movables[0].Move(10);
        movables[1].Move(13);
        Console.WriteLine(movables[0].Position);
        Console.WriteLine(movables[1].Position);
    }
}