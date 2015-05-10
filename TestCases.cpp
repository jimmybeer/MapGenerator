#include "TestCase.h"
#include "IPoint.h"
#include "IRect.h"
#include "Helper.h"
#include <iostream>

void TestCase::Run()
{
    std::cout << "Running Point Test Cases:\n";
    TestCase::RunPointTests();
    
    std::cout << "Running Rectangle Test Cases:\n";
    TestCase::RunRectTests();
}

void TestCase::RunPointTests()
{
    int count = 0;
    int pass = 0;
    
    IPoint p1(10,5);
    IPoint p2(p1);
    
    // Copy Constructor
    count++;
    std::cout << "Copy Constructor: ";
    if(p2.X == p1.X && p2.Y == p1.Y)
    {
        pass++;
        std::cout << "PASS\n";
    }
    else
    {
        std::cout << "FAIL - " << p2.toString() << "\n";
    }
    
    IPoint p3(35, 25);
    p2 = p3;
    
    // Assignment operator
    count++;
    std::cout << "Assignment operator: ";
    if(p2.X == p3.X && p2.Y == p3.Y)
    {
        pass++;
        std::cout << "PASS\n";
    }
    else
    {
        std::cout << "FAIL - " << p2.toString() << "\n";
    }
    
    // Addition and Subtraction
    IPoint p4;
    p4 = p1 + p2;
    count++;
    std::cout << "Addition operator: ";
    if(p4.X == 45 && p4.Y == 30)
    {
        pass++;
        std::cout << "PASS\n";
    }
    else
    {
        std::cout << "FAIL - " << p4.toString() << "\n";
    }
    
    p4 = p2 - p1;
    count++;
    std::cout << "Subtraction operator: ";
    if(p4.X == 25 && p4.Y == 20)
    {
        pass++;
        std::cout << "PASS\n";
    }
    else
    {
        std::cout << "FAIL - " << p4.toString() << "\n";
    }
    
    // Multiplation and Divide
    p4 = p1 * 5;
    count++;
    std::cout << "Multiplation operator: ";
    if(p4.X == 50 && p4.Y == 25)
    {
        pass++;
        std::cout << "PASS\n";
    }
    else
    {
        std::cout << "FAIL - " << p4.toString() << "\n";
    }
    
    p4 = p2 / 5;
    count++;
    std::cout << "Divide operator: ";
    if(p4.X == 7 && p4.Y == 5)
    {
        pass++;
        std::cout << "PASS\n";
    }
    else
    {
        std::cout << "FAIL - " << p4.toString() << "\n";
    }
    
    // Zero and IsNull
    count++;
    std::cout << "!IsNull: ";
    if(p2.IsNull() == false)
    {
        pass++;
        std::cout << "PASS\n";
    }
    else
    {
        std::cout << "FAIL - " << p2.toString() << "\n";
    }
    
    count++;
    std::cout << "Zero().IsNull: ";
    if(p2.Zero().IsNull() == true)
    {
        pass++;
        std::cout << "PASS\n";
    }
    else
    {
        std::cout << "FAIL - " << p2.toString() << "\n";
    }
    
    std::cout << "Completed (" + std::to_string(pass) + "/" + std::to_string(count) + ")\n\n";
}

void TestCase::RunRectTests()
{
    int count = 0;
    int pass = 0;
    
    IRect r1;
    
    // IsNull
    count++;
    std::cout << "IsNull: ";
    if(r1.IsNull() == true)
    {
        pass++;
        std::cout << "PASS\n";
    }
    else
    {
        std::cout << "FAIL - " << r1.toString() << "\n";
    }
    
    IRect r2(10, 20, 40, 40);
    
    IRect r3(r2);
    
    // Copy Constructor
    count++;
    std::cout << "Copy Constructor: ";
    if((r3.Position.X == r2.Position.X) && (r3.Position.Y == r2.Position.Y)
       && (r3.Width == r2.Width) && (r3.Height == r2.Height))
    {
        pass++;
        std::cout << "PASS\n";
    }
    else
    {
        std::cout << "FAIL - " << r3.toString() << "\n";
    }
    
    r1 = r3;
    
    // Assignment operator
    count++;
    std::cout << "Assignment operator: ";
    if((r1.Position.X == r3.Position.X) && (r1.Position.Y == r3.Position.Y)
       && (r1.Width == r3.Width) && (r1.Height == r3.Height))
    {
        pass++;
        std::cout << "PASS\n";
    }
    else
    {
        std::cout << "FAIL - " << r1.toString() << "\n";
    }
    
    r2.Position.X = 30;
    r2.Position.Y = 40;
    r2.Height = 60;
    
    r3.Position.X = 20;
    r3.Position.Y = 65;
    r3.Width = 20;
    r3.Height = 20;
    
    // Positional tests
    count++;
    std::cout << "Half Width: ";
    if(r2.HalfWidth() == 20)
    {
        pass++;
        std::cout << "PASS\n";
    }
    else
    {
        std::cout << "FAIL - " << r2.toString() << "\n";
    }
    
    count++;
    std::cout << "Half Height: ";
    if(r2.HalfHeight() == 30)
    {
        pass++;
        std::cout << "PASS\n";
    }
    else
    {
        std::cout << "FAIL - " << r2.toString() << "\n";
    }
    
    count++;
    std::cout << "CenterX: ";
    if(r2.CenterX() == 50)
    {
        pass++;
        std::cout << "PASS\n";
    }
    else
    {
        std::cout << "FAIL - " << r2.toString() << "\n";
    }
    
    count++;
    std::cout << "CenterY: ";
    if(r2.CenterY() == 70)
    {
        pass++;
        std::cout << "PASS\n";
    }
    else
    {
        std::cout << "FAIL - " << r2.toString() << "\n";
    }
    
    count++;
    std::cout << "Left: ";
    if(r3.Left() == 20)
    {
        pass++;
        std::cout << "PASS\n";
    }
    else
    {
        std::cout << "FAIL - " << r3.toString() << "\n";
    }
    
    count++;
    std::cout << "Right: ";
    if(r3.Right() == 40)
    {
        pass++;
        std::cout << "PASS\n";
    }
    else
    {
        std::cout << "FAIL - " << r3.toString() << "\n";
    }
    
    count++;
    std::cout << "Top: ";
    if(r3.Top() == 65)
    {
        pass++;
        std::cout << "PASS\n";
    }
    else
    {
        std::cout << "FAIL - " << r3.toString() << "\n";
    }
    
    count++;
    std::cout << "Bottom: ";
    if(r3.Bottom() == 85)
    {
        pass++;
        std::cout << "PASS\n";
    }
    else
    {
        std::cout << "FAIL - " << r3.toString() << "\n";
    }
    
    count++;
    std::cout << "Contains: ";
    if(r1.Contains(20, 30))
    {
        pass++;
        std::cout << "PASS\n";
    }
    else
    {
        std::cout << "FAIL - " << r1.toString() << "\n";
    }
    
    IPoint p(60, 50);
    
    count++;
    std::cout << "!Contains: ";
    if(r1.Contains(p) == false)
    {
        pass++;
        std::cout << "PASS\n";
    }
    else
    {
        std::cout << "FAIL - " << r1.toString() << "\n";
    }
    
    count++;
    std::cout << "IsIntersecting: ";
    if(r1.IsIntersecting(r2) && r2.IsIntersecting(r1))
    {
        pass++;
        std::cout << "PASS\n";
    }
    else
    {
        std::cout << "FAIL - " << r1.toString() << "<>" << r2.toString() << "\n";
    }
    
    count++;
    std::cout << "!IsIntersecting: ";
    if(r1.IsIntersecting(r3) == false && r3.IsIntersecting(r1) == false)
    {
        pass++;
        std::cout << "PASS\n";
    }
    else
    {
        std::cout << "FAIL - " << r1.toString() << "<>" << r3.toString() << "\n";
    }
    
    count++;
    std::cout << "Intersect: ";
    IRect* r4 = r2.Intersect(r1);
    if(r4)
    {
        if((r4->Position.X == 30) && (r4->Position.Y == 40)
           && (r4->Width == 20) && (r4->Height == 20))
        {
            pass++;
            std::cout << "PASS\n";
        }
        else
        {
            std::cout << "FAIL - " << r4->toString() << "\n";
        }
    }
    else
    {
        std::cout << "FAIL - Intersect returned NULL\n";
    }
    
    delete r4;
    
    count++;
    std::cout << "IntersectWith: ";
    r3.IntersectWith(r2);
    if((r3.Position.X == 30) && (r3.Position.Y == 65)
       && (r3.Width == 10) && (r3.Height == 20))
    {
        pass++;
        std::cout << "PASS\n";
    }
    else
    {
        std::cout << "FAIL - " << r3.toString() << "\n";
    }
    
    count++;
    std::cout << "United: ";
    r4 = r1.United(r2);
    if(r4)
    {
        if((r4->Position.X == 10) && (r4->Position.Y == 20)
           && (r4->Width == 60) && (r4->Height == 80))
        {
            pass++;
            std::cout << "PASS\n";
        }
        else
        {
            std::cout << "FAIL - " << r4->toString() << "\n";
        }
    }
    else
    {
        std::cout << "FAIL - United returned NULL\n";
    }
    
    delete r4;
    
    count++;
    std::cout << "Penetration: ";
    IPoint* penetration = r1.Penetration(r2);
    if(penetration)
    {
        if((penetration->X == -20) && (penetration->Y == -20))
        {
            pass++;
            std::cout << "PASS\n";
        }
        else
        {
            std::cout << "FAIL - " << p.toString() << "\n";
        }
    }
    else 
    {
        std::cout << "FAIL - penetration returned NULL\n";
    }
    
    delete penetration;
    
    std::cout << "Completed (" + std::to_string(pass) + "/" + std::to_string(count) + ")\n\n";
}

    