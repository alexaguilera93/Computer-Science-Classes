import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;
import java.util.StringTokenizer;

class Neighbor{
	public int vertixNum;
	public Neighbor next;
	public Neighbor(int vnum, Neighbor ngbr){
		this.vertixNum = vnum;
		next = ngbr;
	}
}
class Vertex{
	String name;
	Neighbor adjList;
	String school;
	Boolean enrolled;
	Vertex(String name, Neighbor neighbors){
		this.name = name;
		this.adjList = neighbors;
	}
}
public class Graph {
	Vertex[] adjLists;
	
	public Graph(String file) throws FileNotFoundException {
		
		Scanner sc = new Scanner(new File(file));
		adjLists = new Vertex[sc.nextInt()];

		for (int v=0; v < adjLists.length; v++) {
			String breakup = sc.nextLine();
			StringTokenizer k = new StringTokenizer(breakup, "|");
			adjLists[v] = new Vertex(k.nextToken(), null);
			if(k.nextToken().equals("y")){
				adjLists[v].enrolled = true;
				adjLists[v].school = k.nextToken();
			}else{
				adjLists[v].enrolled = false;
			}
			}
		while(sc.hasNextLine()){
		StringTokenizer l = new StringTokenizer(sc.nextLine(), "|");
		int v1 = findInList(l.nextToken(), adjLists);
		int v2 = findInList(l.nextToken(), adjLists);
		adjLists[v1].adjList = new Neighbor(v2, adjLists[v1].adjList);
		adjLists[v1].adjList = new Neighbor(v1, adjLists[v2].adjList);
		}

		
	}
	int findInList(String Name, Vertex[] list){
		for(int i = 0; i < list.length; i++){
			if(list[i].name.equals(Name)){
				return i;
			}
		}
		return -1;
	}
}