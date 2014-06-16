package poly;


import java.io.*;
import java.util.NoSuchElementException;
import java.util.StringTokenizer;


/**
 * This class implements a term of a polynomial.
 * 
 * @author runb-cs112
 *
 */
class Term {
	/**
	 * Coefficient of term.
	 */
	public float coeff;
	
	/**
	 * Degree of term.
	 */
	public int degree;
	
	/**
	 * Initializes an instance with given coefficient and degree.
	 * 
	 * @param coeff Coefficient
	 * @param degree Degree
	 */
	public Term(float coeff, int degree) {
		this.coeff = coeff;
		this.degree = degree;
	}
	
	/* (non-Javadoc)
	 * @see java.lang.Object#equals(java.lang.Object)
	 */
	public boolean equals(Object other) {
		return other != null &&
		other instanceof Term &&
		coeff == ((Term)other).coeff &&
		degree == ((Term)other).degree;
	}
	
	/* (non-Javadoc)
	 * @see java.lang.Object#toString()
	 */
	public String toString() {
		if (degree == 0) {
			return coeff + "";
		} else if (degree == 1) {
			return coeff + "x";
		} else {
			return coeff + "x^" + degree;
		}
	}
}

/**
 * This class implements a linked list node that contains a Term instance.
 * 
 * @author runb-cs112
 *
 */
class Node {
	
	/**
	 * Term instance. 
	 */
	Term term;
	
	/**
	 * Next node in linked list. 
	 */
	Node next;
	
	/**
	 * Initializes this node with a term with given coefficient and degree,
	 * pointing to the given next node.
	 * 
	 * @param coeff Coefficient of term
	 * @param degree Degree of term
	 * @param next Next node
	 */
	public Node(float coeff, int degree, Node next) {
		term = new Term(coeff, degree);
		this.next = next;
	}
}

/**
 * This class implements a polynomial.
 * 
 * @author runb-cs112
 *
 */
public class Polynomial {
	
	/**
	 * Pointer to the front of the linked list that stores the polynomial. 
	 */ 
	Node poly;
	
	/** 
	 * Initializes this polynomial to empty, i.e. there are no terms.
	 *
	 */
	public Polynomial() {
		poly = null;
	}
	
	/**
	 * Reads a polynomial from an input stream (file or keyboard). The storage format
	 * of the polynomial is:
	 * <pre>
	 *     <coeff> <degree>
	 *     <coeff> <degree>
	 *     ...
	 *     <coeff> <degree>
	 * </pre>
	 * with the guarantee that degrees will be in descending order. For example:
	 * <pre>
	 *      4 5
	 *     -2 3
	 *      2 1
	 *      3 0
	 * </pre>
	 * which represents the polynomial:
	 * <pre>
	 *      4*x^5 - 2*x^3 + 2*x + 3 
	 * </pre>
	 * 
	 * @param br BufferedReader from which a polynomial is to be read
	 * @throws IOException If there is any input error in reading the polynomial
	 */
	public Polynomial(BufferedReader br) throws IOException {
		String line;
		StringTokenizer tokenizer;
		float coeff;
		int degree;
		
		poly = null;
		
		while ((line = br.readLine()) != null) {
			tokenizer = new StringTokenizer(line);
			coeff = Float.parseFloat(tokenizer.nextToken());
			degree = Integer.parseInt(tokenizer.nextToken());
			poly = new Node(coeff, degree, poly);
		}
	}
	
	
	/**
	 * Returns the polynomial obtained by adding the given polynomial p
	 * to this polynomial - DOES NOT change this polynomial
	 * 
	 * @param p Polynomial to be added
	 * @return A new polynomial which is the sum of this polynomial and p.
	 */
	public Polynomial add(Polynomial p){
		Polynomial newOne = new Polynomial();
		Polynomial t = new Polynomial();
		Polynomial pn = new Polynomial();
		t.poly = this.poly;
		pn = p;
		Node x;
		while(pn.poly!=null || t.poly!= null){
			if(pn.poly == null)
				t.poly = addAfterPol(t,newOne);
			else if (t.poly == null)
				pn.poly = addAfterPol(pn, newOne);
			else if(t.poly.term.degree < pn.poly.term.degree)
				t.poly = addAfterPol(t,newOne);
			else if(t.poly.term.degree == pn.poly.term.degree){
				x = newOne.poly;
				if(newOne.poly == null)
					newOne.poly = new Node(pn.poly.term.coeff + t.poly.term.coeff, pn.poly.term.degree, newOne.poly);
				else{
					while(x != null){
						if(pn.poly.term.coeff + t.poly.term.coeff == 0)
							break;
						if(x.next == null){
							x.next = new Node(pn.poly.term.coeff + t.poly.term.coeff, t.poly.term.degree, null);
							break;
						}
						x = x.next;
					}
				}
				pn.poly = pn.poly.next;
				t.poly = t.poly.next;
			}
			else
				pn.poly = addAfterPol(pn, newOne);
		}
		return newOne;
	}
	
	private Node addAfterPol(Polynomial affec, Polynomial newItem){
		Node temp = newItem.poly;
		if(newItem.poly == null)
			newItem.poly = new Node(affec.poly.term.coeff, affec.poly.term.degree, newItem.poly);
		else{
			while(temp != null){
				if(temp.next == null){
					temp.next = new Node(affec.poly.term.coeff, affec.poly.term.degree, null);
					break;
				}
				temp = temp.next;
			}
		}
		return affec.poly.next;	
	}
	/**
	 * Returns the polynomial obtained by multiplying the given polynomial p
	 * with this polynomial - DOES NOT change this polynomial
	 * 
	 * @param p Polynomial with which this polynomial is to be multiplied
	 * @return A new polynomial which is the product of this polynomial and p.
	 */
	public Polynomial multiply(Polynomial p) {
		Polynomial mult = new Polynomial();
		Node c = this.poly;
		Node mc = mult.poly;
		boolean b = false;
		int highestNum = 0;
		while(this.poly != null && p.poly != null){
			if(this.poly.term.degree + p.poly.term.degree > highestNum)
				highestNum = this.poly.term.degree + p.poly.term.degree;
			if(mult.poly == null)
				mult.poly = new Node(p.poly.term.coeff*this.poly.term.coeff, this.poly.term.degree + p.poly.term.degree, mult.poly);
			else{
				while(mc != null){
					if(mc.term.degree == this.poly.term.degree + p.poly.term.degree){
						mc.term.coeff += (p.poly.term.coeff*this.poly.term.coeff);
						b = true;
						break;
					}
					mc = mc.next;
				}
				mc = mult.poly;
				if(!b){
					while(mc != null){
						if(mc.next == null){
							mc.next = new Node(p.poly.term.coeff*this.poly.term.coeff, this.poly.term.degree + p.poly.term.degree, null);
							break;
						}
						
						mc = mc.next;
					}
				}
			}
			if(this.poly.next == null){
				p.poly = p.poly.next;
				this.poly = c;
			}
			else
				this.poly = this.poly.next;
			b = false;
			mc = mult.poly;
		}
		return sort(mult, highestNum);
	}
	
	 private Polynomial sort(Polynomial m, int k) {
		 System.out.print(k);
		 	Polynomial ll = new Polynomial();
	        Polynomial pp = new Polynomial();
	        ll.poly = m.poly;
	        Node rand = m.poly;
	        int deg = 0;
	        int deg2 = k+1;
	        float co = 0;
	        while(rand != null){
	        	while(ll.poly != null){
	        		if(ll.poly.term.degree >= deg && ll.poly.term.degree < deg2){
	        			deg = ll.poly.term.degree;
	        			co = ll.poly.term.coeff;
	        		}
	        		ll.poly = ll.poly.next;
	        	}
	        	pp.poly = new Node(co, deg, pp.poly);
	        	ll.poly = m.poly;
	        	deg = 0;
	        	deg2--;
	        	rand = rand.next;
	        }
	        return pp;
	    }
	
	
	/**
	 * Evaluates this polynomial at the given value of x
	 * 
	 * @param x Value at which this polynomial is to be evaluated
	 * @return Value of this polynomial at x
	 */
	public float evaluate(float x) {
		/** COMPLETE THIS METHOD **/
		float z = 0;
		Polynomial holder = new Polynomial();
		holder = this;
		while(holder.poly != null){
			z += holder.poly.term.coeff * (float)(Math.pow(x,holder.poly.term.degree));
			holder.poly=poly.next;
		}
		return z;
	}

	
	/* (non-Javadoc)
	 * @see java.lang.Object#toString()
	 */
	public String toString() {
		String retval;
		
		if (poly == null) {
			return "0";
		} else {
			retval = poly.term.toString();
			for (Node current = poly.next ;
			current != null ;
			current = current.next) {
				retval = current.term.toString() + " + " + retval;
			}
			return retval;
		}
	}
}