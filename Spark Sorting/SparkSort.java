import org.apache.spark.SparkConf;
import org.apache.spark.api.java.JavaSparkContext;
import org.apache.spark.api.java.JavaPairRDD;
import org.apache.spark.api.java.JavaRDD;
import org.apache.spark.api.java.function.PairFunction;
import org.apache.spark.api.java.function.FlatMapFunction;
import org.apache.spark.api.java.JavaSparkContext;
import org.apache.spark.sql.SparkSession;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.io.Serializable;
import scala.Tuple2;

public class SparkSort {


	public static void main (String[] args) {
	
	String InputFile = args[0];
	String OutputFile = args[1];
	SparkSession spark = SparkSession.builder().appName("SparkSort").getOrCreate();
	SparkConf conf = new SparkConf().setAppName("SparkSort");
	JavaSparkContext sc = new JavaSparkContext(spark.sparkContext());
	
	JavaRDD<String> inputline = sc.textFile(InputFile);

	JavaPairRDD<String, String> keyPairs = inputline.mapToPair(new PairFunction<String,String,String>() {
                public Tuple2<String, String> call(String s){
                        return new Tuple2(s.substring(0,10),s.substring(10));
                }
		}
		).sortByKey(true);

	JavaRDD<String> output = keyPairs.flatMap(new FlatMapFunction<Tuple2<String, String>, String>() {

			public Iterator<String> call(Tuple2<String, String> t) throws Exception {
				List<String> r_Value = new ArrayList<String>();
				r_Value.add(t._1() + "  " + t._2().trim() + "\t");
				return r_Value.iterator();
			}
		});
//	JavaPairRDD<String, String> sortRdd = keyPairs.repartitonAndSortWithinPartition();
	
	output.saveAsTextFile(OutputFile);
	
	sc.stop();
	spark.stop();
	
	}
}

