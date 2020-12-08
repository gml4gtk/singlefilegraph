class main extends c2jrt
{
   private static String class_name = "main";


   static Class pcmain[]=
   {
   java.lang.Integer.TYPE,java.lang.Integer.TYPE
   }
   ;
   public static int cmain;

   static
   {
    try
   {
      cmain=getMethod(Class.forName(class_name),"cmain",pcmain);
   }catch(Exception ex){
    ex.printStackTrace();
   }
   }
static int strmain2= jtocstr("The Java version of sfg.c version %d is running\n");
   
   /* file main.c line 6*/
   
   public static int cmain(int  cargc,int  cargv)
   {
      nextlevel();
      if(true)
      {
      int label= 0;
      int retval= 0;
         
         {
         startargs(
         /* file main.c line 7*/
         sfg.csfg_version());
         
         /* file main.c line 7*/
         cprintf(strmain2,endargs());
         retval= 0;
         if(true)
         {
         prevlevel();
         return retval;
         };
         
         }
      };
      prevlevel();
      return 0;
   }

   public static void main(String[] args)
   throws Exception
   {
      int argc=args.length;
      int argv;
      
      nextlevel();
      argv = makeargv(new Object(), args, argc);
      c_c2jstartup ();
      {
      cmain(argc+1,argv);
      }
      c_c2jexit();
      prevlevel();
   
   }
}
