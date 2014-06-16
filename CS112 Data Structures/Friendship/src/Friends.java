import java.util.NoSuchElementException;
import java.util.Scanner;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;

public class Friends {
	Vertex[] list; 
	
	public static void UserInterface() throws FileNotFoundException{
		Scanner sc = new Scanner(System.in);
		System.out.println("please input name of the file:");
		String File = sc.nextLine();
		build(File);
		
	}
	public static void build(String File) throws FileNotFoundException{
		Graph built = new Graph(File);
		print(built);
			
	
	}
	public static void print(Graph printThis){
		for(int i = 0; i < printThis.adjLists.length; i++){
			Vertex begin = printThis.adjLists[i];
			while(begin.adjList != null){
			String name = begin.name;
			if(begin.enrolled == true){
				String school = begin.school;
				System.out.print(name + " " + school + "->");
			}
			else{
				System.out.print(name + " " + " ->");
			}
			begin.adjList = begin.adjList.next;
			
			
			}
		}
	}
	public static void main(String[] args) throws FileNotFoundException{
		UserInterface();
	}
}