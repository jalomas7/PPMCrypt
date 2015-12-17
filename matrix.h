//Jacob Massengill matrix.h lab4
//this header file defines a 2d array based object capable
//of array operator indexing

#ifndef MATRIX_H
#define MATRIX_H
#include <cstdlib>

template <typename Tbuf>
class matrix {
	public:
		matrix() { buf = NULL; Nrows=0; Ncols=0; }
		~matrix() {
			if (buf) {
				if (buf[0]) delete [] buf[0];
				delete [] buf;
			}
		}

		void assign(int new_Nrows, int new_Ncols) {
			Nrows = new_Nrows;
			Ncols = new_Ncols;
			buf = new Tbuf *[Nrows];
			buf[0] = new Tbuf [Nrows*Ncols];
			for (int i=1; i<Nrows; i++)
				buf[i] = buf[i-1] + Ncols;
		}

		int get_nrows() const 	 { return Nrows; }
		int get_ncols() const 	 { return Ncols; }
		
		Tbuf *data() 	         { return *buf;  }
		Tbuf *operator[] (int i) { return buf[i]; }

	private:
		int Nrows, Ncols;
		Tbuf **buf;
};
#endif // MATRIX_H
