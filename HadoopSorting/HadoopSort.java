import java.io.IOException;
import org.apache.hadoop.conf.Configured;
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.FileStatus;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.Partitioner;
import org.apache.hadoop.mapreduce.lib.partition.TotalOrderPartitioner;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import java.io.*;
import java.lang.Long;
import org.apache.hadoop.util.Tool;
import org.apache.hadoop.util.ToolRunner;
import org.apache.hadoop.mapred.lib.InputSampler;

public class HadoopSort extends Configured implements Tool {


	public static void main(String[] args) throws Exception {
		// TODO Auto-generated method stub
			
		long startTime,endTime;
		startTime = System.currentTimeMillis();
	
		int toolRun = ToolRunner.run(new Configuration(), new HadoopSort(),args);			
		System.exit(toolRun);
	}
	

	@Override
	public int run(String[] args) throws Exception {
 
        	// Configuration 
		Path InputPath = new Path(args[0]);			
		Configuration conf = this.getConf();
	        // Create job
        	Job job = Job.getInstance(conf, "Hadoop Tool runner");
		
		FileInputFormat.addInputPath(job,InputPath);
		FileOutputFormat.setOutputPath(job,new Path(args[1]));			
				
		job.setJarByClass(HadoopSort.class);
//////
		job.setOutputKeyClass(Text.class);
		job.setOutputValueClass(Text.class);
		job.setMapOutputKeyClass(Text.class);
		job.setMapOutputValueClass(Text.class);
//////
		job.setMapperClass(HadoopMapper.class);				
		job.setCombinerClass(HadoopReducer.class);
		job.setReducerClass(HadoopReducer.class);
		
		return job.waitForCompletion(true) ? 0 : 1;
	}
 
}

class HadoopMapper extends Mapper<LongWritable, Text, Text, Text> {
		
		Text lineString1 = new Text();
		Text lineString2 = new Text();
		public HadoopMapper() {}
		
		public void map(LongWritable key,Text value,Context context) throws IOException, InterruptedException{
			
			String str = value.toString();
			lineString1.set(str.substring(0,10));
			lineString2.set(str.substring(10));
			context.write(lineString1,lineString2);
		
	}
	
}
	
class HadoopReducer extends Reducer<Text, Text, Text, Text> {
		
		 Text inputKey = new Text();
		 Text inputValue = new Text();
		 
		public HadoopReducer() {}
		 
		public void reduce(Text key,Text value,Context context) throws IOException,InterruptedException{
			
			inputKey.set(key.toString() + value.toString());     
			inputValue.set("");			
			context.write(inputKey, inputValue);
	}
}

