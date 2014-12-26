package sample;

import java.io.*;
import java.util.StringTokenizer;


public class Main {


    private static int[] user;
    private static int[] movie;
    private static int[] raiting;
    private static double[] bu;
    private static double[] bi;
    private static double[][] pu;
    private static double[][] qi;

    private static int count = 0;
    private static double med;
    private static double sum = 1.9;
    private static double prevSum = 2;
    private static int paramsCount = 4;
    private static int maxRaiting;
    private static int trains;
    private static int tests;
    private static int users;
    private static int movies;
    private static BufferedReader br;

    private void run() throws IOException {
        br = new BufferedReader(new FileReader("input.txt"), 1024);
        readData();
        train();
        test();
    }

    public static void main(String[] args) throws IOException {
        new Main().run();
    }

    private void readData() throws IOException {
        StringTokenizer st;
        st = new StringTokenizer(br.readLine());
        maxRaiting = Integer.parseInt(st.nextToken());
        users = Integer.parseInt(st.nextToken());
        movies = Integer.parseInt(st.nextToken());
        trains = Integer.parseInt(st.nextToken());
        tests = Integer.parseInt(st.nextToken());
        qi = new double[movies][paramsCount];
        pu = new double[users][paramsCount];
        bu = new double[users];
        bi = new double[movies];
        user = new int[trains];
        movie = new int[trains];
        raiting = new int[trains];
        for (int idx = 0; idx < trains; ++idx) {
            count++;
            st = new StringTokenizer(br.readLine(), " ");
            user[idx] = Integer.parseInt(st.nextToken());
            movie[idx]  = Integer.parseInt(st.nextToken());
            raiting[idx]  = Integer.parseInt(st.nextToken());
            med += raiting[idx] ;
            bu[user[idx]] = 0.1;
            bi[movie[idx]] = 0.1;
            qi[movie[idx]] = makeParams();
            pu[user[idx]] = makeParams();
        }
        med /= count;
    }

    private static double[] makeParams() {
        double[] random = new double[paramsCount];
        for (int i = 0; i < paramsCount; i++) {
            random[i] = Math.random();
        }
        return random;
    }

    private static void train() {
        double lambda = 0.02;
        double gamma = 0.005;
        double rui = 0;
        double eui;
        for (int idx = 0; idx < 300 && Math.abs(sum - prevSum) > 0.0001; idx++) {
            prevSum = sum;
            sum = 0;
            for(int y = 0;y < count;y++){
                rui = med + bi[movie[y]] + bu[user[y]] + qtp(movie[y], user[y]);
                eui = raiting[y] - rui;
                sum += eui * eui;
                bu[user[y]] += gamma * (eui - lambda * bu[user[y]]);
                bi[movie[y]] += gamma * (eui - lambda * bi[movie[y]]);
                for (int i = 0; i < paramsCount; i++) {
                    double oldPu = pu[user[y]][i];
                    double oldQi = qi[movie[y]][i];
                    pu[user[y]][i] += gamma * (eui * oldQi - lambda * oldPu);
                    qi[movie[y]][i] += gamma * (eui * oldPu - lambda * oldQi);
                }
            }
            sum = Math.sqrt(sum / count);
        }
    }

    private static void test() throws IOException {
        PrintWriter pw = new PrintWriter(("output.txt"));
        StringTokenizer st;
        String s;
        int user;
        int movie;
        double rui;
        while ((s = br.readLine()) != null) {
            st = new StringTokenizer(s, " ");
            user = Integer.parseInt(st.nextToken());
            movie = Integer.parseInt(st.nextToken());
            rui = med;
            if (movie < movies) {
                rui += bi[movie];
            }
            if (user < users) {
                rui += bu[user];
            }
            if (user < users && movie < movies) {
                rui += qtp(movie, user);
            }
            rui = Math.min(rui, maxRaiting);
            pw.println(rui);
        }
        pw.close();

    }

    private static double qtp(int movie, int user) {
        double result = 0;
        for (int i = 0; i < paramsCount; i++) {
            result += qi[movie][i] * pu[user][i];
        }
        return result;
    }
}