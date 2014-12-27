import java.util.*;
import java.io.*;

class Main {

    static final int[][] steps = {
            {-1, 0}, {1, 0}, {0, -1}, {0, 1}
    };

    static final boolean check(int index, int lim){
        return (0 <= index && index < lim);
    }

    private static final char EMPTY_CELL = '*';

    private static int firstSize, secondSize;
    private static List<Integer>[] graph;

    private static int[] matched;
    private static boolean[] marked;

    private static int[] used;
    private static boolean[] firstEmpty, secondEmpty;


    private static void solve() throws IOException {
        int rows = readInt();
        int columns = readInt();

        int doubleCost = readInt();
        int singleCost = readInt();

        char[][] field = new char[rows][];
        for (int i = 0; i < rows; ++i) {
            field[i] = in.readLine().toCharArray();
        }

        firstSize = 0;
        secondSize = 0;

        int[][] graphIndexes = new int[rows][columns];

        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < columns; ++j) {
                if (i % 2 == j % 2) {
                    graphIndexes[i][j] = firstSize++;
                } else {
                    graphIndexes[i][j] = secondSize++;
                }
            }
        }

        firstEmpty = new boolean[firstSize];
        secondEmpty = new boolean[secondSize];

        matched = new int[secondSize];
        Arrays.fill(matched, -1);

        marked = new boolean[firstSize];
        Arrays.fill(marked, false);

        graph = new List[firstSize];
        for (int v = 0; v < firstSize; ++v) {
            graph[v] = new ArrayList<Integer>();
        }

        for (int i = 0; i < rows; ++i) {
            char[] row = field[i];

            for (int j = 0; j < columns; ++j) {
                int index = graphIndexes[i][j];
                boolean isEmpty = (row[j] == EMPTY_CELL);

                if (i % 2 == j % 2) {
                    firstEmpty[index] = isEmpty;
                    for (int[] step : steps) {
                        int x = i + step[0];
                        int y = j + step[1];

                        if (check(x, rows) && check(y, columns) && field[x][y] == EMPTY_CELL) {
                            graph[index].add(graphIndexes[x][y]);
                        }
                    }
                } else {
                    secondEmpty[index] = isEmpty;
                }
            }
        }

        if (doubleCost < 2 * singleCost) {
            used = new int[firstSize];
            Arrays.fill(used, -1);
            for (int v = 0; v < firstSize; ++v) {
                if (firstEmpty[v]) {
                    marked[v] = greedyKhun(v);
                }
            }
            for (int v = 0; v < firstSize; ++v) {
                if (firstEmpty[v] && !marked[v]) {
                    marked[v] = khun(v, v);
                }
            }
        }

        int totalCost = 0;
        for (int v = 0; v < secondSize; ++v) {
            if (secondEmpty[v]) {
                if (matched[v] != -1) {
                    totalCost += doubleCost;
                } else {
                    totalCost += singleCost;
                }
            }
        }

        for (int v = 0; v < firstSize; ++v) {
            if (firstEmpty[v] && !marked[v]) {
                totalCost += singleCost;
            }
        }

        out.println(totalCost);
    }

    private static boolean greedyKhun(int from) {
        for (int to : graph[from]) {
            if (matched[to] == -1) {
                matched[to] = from;
                return true;
            }
        }

        return false;
    }

    private static boolean khun(int from, int color) {
        if (used[from] == color) return false;
        used[from] = color;

        for (int to : graph[from]) {
            if (matched[to] == -1 || khun(matched[to], color)) {
                matched[to] = from;
                return true;
            }
        }

        return false;
    }

    private static BufferedReader in;
    private static PrintWriter out;
    private static StringTokenizer tok = new StringTokenizer("");

    public static void main(String[] args) throws IOException{
        new Main().run();
    }

    public void run() throws IOException{
        in = new BufferedReader(new InputStreamReader(System.in));
        out = new PrintWriter(System.out);
        solve();
        out.close();

    }

    private static String readString() throws IOException{
        while(!tok.hasMoreTokens()){
            try{
                tok = new StringTokenizer(in.readLine());
            }catch (Exception e){
                return null;
            }
        }

        return tok.nextToken();
    }

    private static int readInt() throws IOException {
        return Integer.parseInt(readString());
    }
}