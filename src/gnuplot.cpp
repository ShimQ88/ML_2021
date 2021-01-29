#include "gnuplot.h"

void run_gnuplot(int technique){
    Gnuplot gp;
    ifstream f_plot;
    // f_plot.open ("plot/command.txt");
    if(technique==0){
      f_plot.open ("plot/command_ANN.txt");
    }else{
      f_plot.open ("plot/command_RF.txt");
    }
    string line;
    if(f_plot.is_open())
    {
        while (getline(f_plot,line))
        {
          cout << line << '\n';
          // cout<<line<<endl;
          gp(line);
          // getchar();
        }
        f_plot.close();
    }
}