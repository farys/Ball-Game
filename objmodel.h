class Vector
{
public:
    float x, y, z, w;

        Vector(float x, float y, float z, float w)
        {
            this.x = x;
            this.y = y;
            this.z = z;
            this.w = w;
        }

        Vector(float x, float y, float z)
        {
            this.x = x;
            this.y = y;
            this.z = z;
            this.w = 0;
        }

        void Normalize()
        {
            float length = this.GetLength();
            this.x /= length;
            this.y /= length;
            this.z /= length;
            this.w /= length;
        }
        float GetLength()
        {
            return (float)Math.Sqrt((x * x) + (y * y) + (z * z) + (w * w));
        }
};

class Vertex : public Vector
{
        public:
        Vector normal;
        int normalPoints;
        Vertex(float x, float y, float z, float w) : base(x, y, z, w)
        {
            this.normal = new Vector(0, 0, 0, 0);
            normalPoints = 0;
        }
};

class TextureCoord
    {
        public TextureCoord(float u, float v, float w)
        {
            this.u = u;
            this.v = v;
            this.w = w;
        }
        public float u, v, w;
    }
    public class Point
    {

        public Point()
        {
            this.normal = null;
            this.textureCoord = null;
        }

        public Vertex vertex;
        public Vector normal;
        public TextureCoord textureCoord;
    }
    public class Index
    {
        public Index()
        {
            this.points = new List<Point>();
        }

        public void calculateNormal()
        {
            Vertex cur;
            Vertex next;
            Vector tmp = new Vector(0, 0, 0);

            for (int i = 0; i < this.points.Count; i++)
            {
                //vertices
                cur = this.points[i].vertex;
                next = this.points[(i+1)%this.points.Count].vertex;

                tmp.x += ((cur.y - next.y) * (cur.z + next.z));
                tmp.y += ((cur.z - next.z) * (cur.x + next.x));
                tmp.z += ((cur.x - next.x) * (cur.y + next.y));
            }
            tmp.Normalize();


            for(int i = 0; i < this.points.Count; i++)
            {
                cur = this.points[i].vertex;
                cur.normal.x += tmp.x;
                cur.normal.y += tmp.y;
                cur.normal.z += tmp.z;
                cur.normalPoints++;
            }
        }

        public List<Point> points;
        public Material material;
    }
    public class Group
    {
        public Group(String name)
        {
            this.name = name;
            this.indices = new List<Index>();
            this.glList = GL.glGenLists(1);
            this.min = new Vector(0, 0, 0);
            this.max = new Vector(0, 0, 0);
        }

        ~Group()
        {

            try
            {
                GL.glDeleteLists(this.glList, 1);
            }
            catch (Exception e)
            {
                e.ToString();
            }
        }

        public void AddIndex(Index index)
        {
            this.indices.Add(index);
        }

        public List<Index> GetIndices()
        {
            return this.indices;
        }

        public String GetName()
        {
            return this.name;
        }

        public override string ToString()
        {
            return this.name;
        }

        public void Compile(bool colors)
        {
            min = new Vector(0, 0, 0);
            max = new Vector(0, 0, 0);
            Random r = new Random();

            GL.glNewList(this.glList, GL.GL_COMPILE);
            foreach (Index index in this.indices)
            {
                GL.glBegin(GL.GL_TRIANGLE_FAN);
                foreach (Point point in index.points)
                {
                    //colors
                    if (colors == true)
                    {
                        //GL.glColor3i(r.Next(100, 255), r.Next(100, 255), r.Next(100, 255));
                        GL.glColor3d((r.NextDouble() * 0.5f) + 0.5f, (r.NextDouble() * 0.5f) + 0.5f, (r.NextDouble() * 0.5f) + 0.5f);
                    }
                    else
                    {
                        GL.glColor3f(1.0f, 1.0f, 1.0f);
                    }
                    if (point.textureCoord != null)
                    {
                        GL.glTexCoord2f(point.textureCoord.u, point.textureCoord.v);
                    }
                    //normals
                    if (point.normal != null)
                    {
                        GL.glNormal3d(point.normal.x, point.normal.y, point.normal.z);
                    }
                    else
                    {
                        GL.glNormal3d(point.vertex.normal.x, point.vertex.normal.y, point.vertex.normal.z);
                    }

                    GL.glVertex3d(point.vertex.x, point.vertex.y, point.vertex.z);

                    min.x = Math.Min(min.x, point.vertex.x);
                    max.x = Math.Max(max.x, point.vertex.x);
                    min.y = Math.Min(min.y, point.vertex.y);
                    max.y = Math.Max(max.y, point.vertex.y);
                    min.z = Math.Min(min.z, point.vertex.z);
                    max.z = Math.Max(max.z, point.vertex.z);
                }
                GL.glEnd();
            }
            GL.glEndList();

        }
        public void Render()
        {
            GL.glCallList(glList);
        }

        protected uint glList;
        String name;
        List<Index> indices;
        public Vector min, max;
    }

    public class Material
    {
        public Material(String name)
        {
            this.name = name;
        }

        public String name;
        //diffuse
        //ambient
        //bla bla
    }

    public class ObjModel
    {
        public List<Vertex> vertices;
        public List<Vector> vertexNormals;
        public List<TextureCoord> textureCoords;
        public List<Material> materials;

        public List<Group> LoadGroups(String filename)
        {
            vertices = new List<Vertex>();
            vertexNormals = new List<Vector>();
            textureCoords = new List<TextureCoord>();
            materials = new List<Material>();
            List<Group> groups = new List<Group>();

            char[] sep = { ' ' };
            if(filename.Equals("")) return new List<Group>();
            StreamReader reader = new StreamReader(filename);
            String line;
            String[] args;

            Material lastUsedMaterial = new Material("Default");
            materials.Add(lastUsedMaterial);
            Group lastGroup = new Group("Default");
            groups.Add(lastGroup);

            while(!reader.EndOfStream)
            {
                line = reader.ReadLine();
                if (line.Length < 2) continue;
                args = line.Split(sep, StringSplitOptions.RemoveEmptyEntries);

                switch (args[0])
                {
                    case "v":
                        vertices.Add(this.parseVertex(args));
                    break;

                    case "vt":
                        textureCoords.Add(this.parseTextureCoord(args));
                    break;

                    case "vn":
                        vertexNormals.Add(this.parseVertexNormal(args));
                    break;

                    case "f":
                        lastGroup.AddIndex(this.parseIndex(args, vertices, textureCoords, vertexNormals, lastUsedMaterial));
                    break;

                    case "usemtl":
                        foreach(Material material in materials)
                        {
                            if(material.name.Equals(args[1]))
                            {
                                lastUsedMaterial = material;
                            }
                        }
                    break;

                    case "g":
                        string name = String.Join(" ", args, 1, args.Length - 1);

                        lastGroup = new Group(name);
                        groups.Add(lastGroup);
                    break;
                }
            }

            //usuwamy puste grupy
            for (int i = 0; i < groups.Count; i++)
            {
                if (groups[i].GetIndices().Count == 0)
                {
                    groups.Remove(groups[i]);
                }
            }

            foreach (Group group in groups)
            {
                group.Compile(false);
            }
            return groups;
        }

        protected Vertex parseVertex(String[] args)
        {
            System.Globalization.CultureInfo info = System.Globalization.CultureInfo.InvariantCulture;
            float w = 0, x, y, z;
            if(args.Length == 5) // w is optimal
            {
                w = float.Parse(args[4], info);
            }

            x = float.Parse(args[1], info);
            y = float.Parse(args[2], info);
            z = float.Parse(args[3], info);

            return new Vertex(x, y, z, w);
        }

        protected Vector parseVertexNormal(String[] args)
        {
            System.Globalization.CultureInfo info = System.Globalization.CultureInfo.InvariantCulture;
            float nx, ny, nz;

            nx = float.Parse(args[1], info);
            ny = float.Parse(args[2], info);
            nz = float.Parse(args[3], info);
            return new Vector(nx, ny, nz);
        }

        protected Index parseIndex(String[] args, List<Vertex> vertices, List<TextureCoord> textureCoords, List<Vector> vertexNormals, Material lastUsedMaterial)
        {
            Index index = new Index();
            index.material = lastUsedMaterial;
            /*
                 * format: f 1/2/1
                 *
                 */
            for (int i = 1; i < args.Length; i++)
            {
                String[] coords = args[i].Split('/');

                Point p = new Point();

                p.vertex = vertices[Int32.Parse(coords[0])-1];
                if ((coords.Length > 1) && (!coords[1].Equals("")))
                {
                    p.textureCoord = textureCoords[Int32.Parse(coords[1])-1];
                }

                if (coords.Length == 3)
                {
                    p.normal = vertexNormals[Int32.Parse(coords[2]) - 1];
                }
                index.points.Add(p);
            }

            index.calculateNormal();
            return index;
        }

        protected TextureCoord parseTextureCoord(String[] args)
        {
            float w = 0, u, v;
            System.Globalization.CultureInfo info = System.Globalization.CultureInfo.InvariantCulture;

            if (args.Length == 4)
            {
                w = float.Parse(args[3], info);
            }

            u = float.Parse(args[1], info);
            v = float.Parse(args[2], info);

            return new TextureCoord(u, v, w);
        }
    }
}
